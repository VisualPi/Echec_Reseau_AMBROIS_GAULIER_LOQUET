//Server

#include <winsock2.h>
#include <iostream>
#include <list>
#include <map>
#include <algorithm>
#include <iterator>

#pragma comment(lib, "ws2_32.lib")

static const int PORT = 12345;
static const int BUF_LEN = 512;

typedef struct Client
{
	SOCKET socket_;
	bool isSpectator_;

	Client(SOCKET& sock, bool spec)
		: socket_(sock), isSpectator_(spec){}
} Client;

enum gameState{ WAITING, STARTED };

struct Game
{
	std::list<Client> players;
	gameState state;
};

bool SetSocketBlocking(SOCKET &sock, bool blocking = true)
{
	unsigned long nonblocking_long = blocking ? 0 : 1;
	if (ioctlsocket(sock, FIONBIO, &nonblocking_long) == SOCKET_ERROR)
		return false;
	return true;
}

int Initialize(SOCKET& sock, SOCKADDR_IN& sin)
{
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

// Un thread par partie
// chaque thread gère le tour à tour de chaque joueur

int main(int, char**) {
	SOCKET serverSocket;
	SOCKADDR_IN serverSockaddr_In;

	Initialize(serverSocket, serverSockaddr_In);

	while(true) {

	}

	int yolo = 0;
	std::cin >> yolo;

	return 0;
}