//Server

#include <winsock2.h>
#include <iostream>
#include <list>
#include <map>
#include <algorithm>
#include <iterator>
#include <vector>
#include <string>

#include "ChessBoard.hpp"

#pragma comment(lib, "ws2_32.lib")

static const int PORT = 12345;
static const int BUF_LEN = 512;
static const std::string SEPARATOR = "|";

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
	}
}

// Client
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
static const std::string NEWPOS_HEADER_PACKET = "NEWPOS";
static const std::string PLAYERTURN_HEADER_PACKET = "PLAYERTURN";
static const std::string PLAYERWON_HEADER_PACKET = "PLAYERWON";
static const std::string FULLBOARD_HEADER_PACKET = "FULLBOARD";

static const std::string MOVE_HEADER_PACKET = "MOVE";
static const std::string CLIENTMODE_HEADER_PACKET = "CLIENTMODE";

// Envoie la nouvelle position (newPos) de la pi�ce � l'ancienne position (oldPos)
std::string CreateNewPositionPacket(Vector2i oldPos, Vector2i newPos) {
	std::string temp = NEWPOS_HEADER_PACKET + SEPARATOR + std::to_string(oldPos.x) + SEPARATOR + std::to_string(oldPos.y) + SEPARATOR + std::to_string(newPos.x) + SEPARATOR + std::to_string(newPos.y);
	return temp;
}

// Dit � qui c'est le tour
std::string CreatePlayerTurnPacket(EColor colorTurn) {
	std::string temp = PLAYERTURN_HEADER_PACKET + SEPARATOR + std::to_string(colorTurn);
	return temp;
}

// Dit qui a gagn�
std::string CreatePlayerWonPacket(EColor colorWon) {
	std::string temp = PLAYERWON_HEADER_PACKET + SEPARATOR + std::to_string(colorWon);
	return temp;
}

// Envoie tout le board
std::string CreateFullChessBoardPacket(ChessBoard chessboard) {
	std::string temp = FULLBOARD_HEADER_PACKET;
	auto pieces = chessboard.getBlackTeam()->GetPieces();
	for(int i = 0; i < pieces->size(); ++i) {
		auto tempPiece = pieces->at(i);
		temp += SEPARATOR + std::to_string((*tempPiece).GetCase().x) + SEPARATOR + std::to_string((*tempPiece).GetCase().y) + SEPARATOR + std::to_string((*tempPiece).GetColor()) + SEPARATOR + std::to_string((*tempPiece).GetType());
	}
	pieces = chessboard.getWhiteTeam()->GetPieces();
	for(int i = 0; i < pieces->size(); ++i) {
		auto tempPiece = pieces->at(i);
		temp += SEPARATOR + std::to_string((*tempPiece).GetCase().x) + SEPARATOR + std::to_string((*tempPiece).GetCase().y) + SEPARATOR + std::to_string((*tempPiece).GetColor()) + SEPARATOR + std::to_string((*tempPiece).GetType());
	}
	return temp;
}

//// Client
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//// Envoie la demande de d�placement
//std::string CreateMovePacket(SOCKET socket, Vector2i piecePos, Vector2i requestedPos) {
//	std::string temp = MOVE_HEADER_PACKET + SEPARATOR + std::to_string(piecePos.x) + SEPARATOR + std::to_string(piecePos.y) + SEPARATOR + std::to_string(requestedPos.x) + SEPARATOR + std::to_string(requestedPos.y);
//	return temp;
//}
//
//// Envoie la demande de play/spectate
//std::string CreateClientModePacket(SOCKET socket, clientMode clientMode) {
//	std::string temp = CLIENTMODE_HEADER_PACKET + SEPARATOR + std::to_string(clientMode);
//	return temp;
//}

void Read(SOCKET socket) {
	while(true) {
		char matchmakingBuffer[512] = {0};
		recv(socket, matchmakingBuffer, sizeof(matchmakingBuffer), 0);

		std::string s = std::string(matchmakingBuffer);
		std::string delimiter = "|";

		size_t pos = 0;
		std::string token;
		while((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			std::cout << token << std::endl;
			s.erase(0, pos + delimiter.length());
		}
		std::cout << s << std::endl;
	}
}

std::vector<Client> clientsUndefined;
std::list<Client> clientsWaitingToPlay;
std::list<Client> clientsWaitingToSpectate;

// Un thread par partie
// chaque thread g�re le tour � tour de chaque joueur
int main(int, char**) {
	SOCKET serverSocket;
	SOCKADDR_IN serverSockaddr_In;

	Initialize(serverSocket, serverSockaddr_In);

	SOCKET tempClient;
	SOCKADDR_IN tempClientSockaddr_In;

	// Tant que toujours lol
	while(true) {
		// On regarde si y'a un nouveau client
		int sinsize = sizeof(tempClientSockaddr_In);
		if((tempClient = accept(serverSocket, (SOCKADDR *) &tempClientSockaddr_In, &sinsize)) != INVALID_SOCKET) {
			clientsUndefined.push_back(Client(tempClient));
			std::cout << "A client connected !" << std::endl;
			//WriteTest(tempClient);
		}

		// On d�finit ce que les clients veulent faire (spectate/jouer)
		std::list<Client> temp;
		if(clientsUndefined.size() > 0) {
			for(auto client : clientsUndefined) {
				if(client.isSpectator_ == PLAY) {
					clientsWaitingToPlay.push_back(client);
					std::cout << "Client chose to play" << std::endl;
				}
				else if(client.isSpectator_ == SPECTATE) {
					clientsWaitingToSpectate.push_back(client);
					std::cout << "Client chose to spectate" << std::endl;
				}
				else {
					temp.push_back(client);
				}
			}
			clientsUndefined.clear();
			clientsUndefined.assign(temp.begin(), temp.end());
		}

		// D�s qu'on a 2 joueurs en attente de jouer, on cr�e une partie
		if(clientsWaitingToPlay.size() == 2) {
			std::cout << "2 Clients wanting to play, starting a game" << std::endl;
			Game game = Game();
			currentGames.push_back(game);
			game.players.push_back(clientsWaitingToPlay.front());
			clientsWaitingToPlay.pop_front();
			game.players.push_back(clientsWaitingToPlay.front());
			clientsWaitingToPlay.pop_front();
			game.chessboard->InitTeam();
		}

		// Si on a des clients en attente de spectate, on les fait spectate un truc (1�re partie en cours et pis c'est tout)
		if(clientsWaitingToSpectate.size() > 0) {
			std::cout << clientsWaitingToSpectate.size() << " Clients wanting to spectate a game, affecting them to a game" << std::endl;
			for(auto client : clientsWaitingToSpectate) {
				currentGames.front().players.push_back(client);
			}
			clientsWaitingToSpectate.clear();
		}
	}

	return 0;
}