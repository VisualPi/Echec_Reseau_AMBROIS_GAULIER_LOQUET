//Server

#include <winsock2.h>
#include <iostream>
#include <list>
#include <map>
#include <algorithm>
#include <iterator>
#include <vector>

#include "ChessBoard.hpp"

#pragma comment(lib, "ws2_32.lib")

static const int PORT = 12345;
static const int BUF_LEN = 512;

enum clientMode { UNKNOWN, SPECTATE, PLAY };

typedef struct Client {
	SOCKET socket_;
	clientMode isSpectator_;

	Client(SOCKET& sock)
		: socket_(sock), isSpectator_(UNKNOWN) {}
} Client;

enum gameState { WAITING, WAITING_FOR_WHITE_MOVE, WAITING_FOR_BLACK_MOVE, FINISHED };

struct Game {
	std::list<Client> players;
	std::vector<Piece*> pieces;
	ChessBoard* chessboard;
	gameState state;

	Game() {
		chessboard = new ChessBoard();
		state = WAITING_FOR_WHITE_MOVE;
	}
};

std::list<Game> currentGames;

bool SetSocketBlocking(SOCKET &sock, bool blocking = true) {
	unsigned long nonblocking_long = blocking ? 0 : 1;
	if(ioctlsocket(sock, FIONBIO, &nonblocking_long) == SOCKET_ERROR)
		return false;
	return true;
}

int Initialize(SOCKET& sock, SOCKADDR_IN& sin) {
	WSADATA WSAData;
	WSAStartup(MAKEWORD(2, 0), &WSAData);

	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);

	if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
		perror("socket()");
		std::cout << WSAGetLastError() << std::endl;
		return WSAGetLastError();
	}
	std::cout << "Server socket created" << std::endl;

	if(!SetSocketBlocking(sock, false)) {
		perror("init non blocking");
		std::cout << WSAGetLastError() << std::endl;
		return WSAGetLastError();
	}
	std::cout << "Server socket set to non blocking" << std::endl;

	if(bind(sock, (SOCKADDR *) &sin, sizeof(sin)) == SOCKET_ERROR) {
		perror("bind()");
		std::cout << WSAGetLastError() << std::endl;
		return WSAGetLastError();
	}
	std::cout << "Server socked bound" << std::endl;

	if(listen(sock, 0) == SOCKET_ERROR) {
		perror("listen()");
		std::cout << WSAGetLastError() << std::endl;
		return WSAGetLastError();
	}
	std::cout << "Server socket listening ..." << std::endl;

	return 0;
}

void PrintError(const char * err) {
	perror(err);
	std::cout << WSAGetLastError() << std::endl;
	exit(WSAGetLastError());
}

int ReadClient(SOCKET sock, char *buffer) {
	int n = 0;

	if((n = recv(sock, buffer, BUF_LEN - 1, 0)) < 0) {
		perror("recv()");
		n = 0;
	}
	buffer[n] = '\0';

	return n;
}

void WriteClient(SOCKET sock, const char *buffer) {
	if(send(sock, buffer, strlen(buffer), 0) < 0) {
		perror("send()");
		exit(errno);
	}
}

std::vector<Client> clientsUndefined;
std::list<Client> clientsWaitingToPlay;
std::list<Client> clientsWaitingToSpectate;

// Un thread par partie
// chaque thread gère le tour à tour de chaque joueur

int main(int, char**) {
	SOCKET serverSocket;
	SOCKADDR_IN serverSockaddr_In;

	Initialize(serverSocket, serverSockaddr_In);

	//ChessBoard* chessBoard = new ChessBoard();
	//chessBoard->InitTeam();

	//std::cout << chessBoard->AskForMovement(Vector2i(0, 6), Vector2i(0, 5), WHITE, true) << std::endl;

	//SOCKET osef;
	//clientsWaitingToPlay.push_back(Client(osef, true));
	//clientsWaitingToPlay.push_back(Client(osef, true));

	SOCKET tempClient;
	SOCKADDR_IN tempClientSockaddr_In;

	// Toujours
	while(true) {
		// On regarde si y'a un nouveau client
		int sinsize = sizeof(tempClientSockaddr_In);
		if((tempClient = accept(serverSocket, (SOCKADDR *) &tempClientSockaddr_In, &sinsize)) != INVALID_SOCKET) {
			clientsUndefined.push_back(Client(tempClient));
			std::cout << "A client connected !" << std::endl;
		}

		// On définit ce qu'ils veulent faire (spectate/jouer)
		std::list<Client> temp;
		if(clientsUndefined.size() > 0) {
			for(auto client : clientsUndefined) {
				if(client.isSpectator_ == PLAY) {
					clientsWaitingToPlay.push_back(client);
				}
				else if(client.isSpectator_ == SPECTATE) {
					clientsWaitingToSpectate.push_back(client);
				}
				else {
					temp.push_back(client);
				}
			}
			clientsUndefined.clear();
			clientsUndefined.assign(temp.begin(), temp.end());
		}

		// Dés qu'on a 2 joueurs en attente de partie, on crée une partie
		if(clientsWaitingToPlay.size() == 2) {
			Game game = Game();
			currentGames.push_back(game);
			game.players.push_back(clientsWaitingToPlay.front());
			clientsWaitingToPlay.pop_front();
			game.players.push_back(clientsWaitingToPlay.front());
			clientsWaitingToPlay.pop_front();
		}

		// Si on a des clients en attente de spectate, on les fait spectate un truc
		if(clientsWaitingToSpectate.size() > 0) {
			for(auto client : clientsWaitingToSpectate) {
				currentGames.front().players.push_back(client);
			}
			clientsWaitingToSpectate.clear();
		}
	}

	return 0;
}