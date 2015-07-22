#include "stdafx.h"
#define _WINSOCK_DEPRECATED_NO_WARNINGS

const int width = 800;
const int heigth = 600;
bool isGameStarted = false;
bool isSpectator = false; //Pour savoir si le joueur spec une partie -> ne pas pouvoir intéragir le board

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <thread>

static const int PORT = 12345;
static const int BUF_LEN = 512;

typedef struct Vector2i {
	int x;
	int y;

	Vector2i(int newX, int newY)
		: x(newX), y(newY) {
	}
} Vector2i;

static const std::string NEWPOS_HEADER_PACKET = "NEWPOS";
static const std::string PLAYERTURN_HEADER_PACKET = "PLAYERTURN";
static const std::string PLAYERWON_HEADER_PACKET = "PLAYERWON";
static const std::string FULLBOARD_HEADER_PACKET = "FULLBOARD";

static const std::string MOVE_HEADER_PACKET = "MOVE";
static const std::string CLIENTMODE_HEADER_PACKET = "CLIENTMODE";

static const std::string SEPARATOR = "|";

enum clientMode {
	UNKNOWN, SPECTATE, PLAY
};

// Client
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
void WriteClient(SOCKET sock, const char *buffer) {
	if(send(sock, buffer, strlen(buffer), 0) < 0) {
		perror("send()");
	}
}

// Envoie la demande de déplacement
std::string CreateMovePacket(SOCKET socket, Vector2i piecePos, Vector2i requestedPos) {
	std::string temp = MOVE_HEADER_PACKET + SEPARATOR + std::to_string(piecePos.x) + SEPARATOR + std::to_string(piecePos.y) + SEPARATOR + std::to_string(requestedPos.x) + SEPARATOR + std::to_string(requestedPos.y);
	return temp;
}

// Envoie la demande de play/spectate
std::string CreateClientModePacket(SOCKET socket, clientMode clientMode) {
	std::string temp = CLIENTMODE_HEADER_PACKET + SEPARATOR + std::to_string(clientMode);
	return temp;
}

void OnButtonPlayOfflineClick();
void OnButtonSpectateClick();
void OnButtonPlayOnlineClick();
void OnButtonOptionsClick();
void OnButtonExitClick();
void drawAll();
void drawReachablePositionsForSelectedPiece(sf::Vector2i position, EColor team);

sf::RenderWindow render_window(sf::VideoMode(width, heigth), "Hello world!", sf::Style::Titlebar); //JE SAIS PAS POURQUOI VIDEOMODE ACCEPETE PAS LES MACCROS
ChessBoard* board = new ChessBoard(width, heigth);

//EColor currentTeam = WHITE;
bool team = false; //WHITE

bool InitializeWSA() {

	bool wsaStatus = false;
	WSADATA wsa;
	int error = WSAStartup(MAKEWORD(2, 2), &wsa);
	if(error < 0) {
		std::cout << "SERVEUR : WSAStartup Failed !" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "SERVEUR : WSAStartup Initialised !" << std::endl;
		wsaStatus = true;
	}
	return wsaStatus;
}

SOCKET serverSocket;
SOCKADDR_IN serverSocketSin;

int CreateSocket() {
	//CREATION DE LA SOCKET CLIENT
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);


	//SETTING DES PARAMETRES DE LA SOCKET CLIENT
	serverSocketSin.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverSocketSin.sin_family = AF_INET;
	serverSocketSin.sin_port = htons(PORT);

	//TEST SI LA CONNECTION EST VALIDE
	if(serverSocket == INVALID_SOCKET) {
		std::cout << "CLIENT : Create Socket Error !" << std::endl;
		return -1;
	}
	else {
		std::cout << "CLIENT : Create Socket Completed !" << std::endl;
		return 0;
	}
}

int ConnectToServer() {
	int erreurConnection = 0;

	erreurConnection = connect(serverSocket, (SOCKADDR *) &serverSocketSin, sizeof(serverSocketSin));

	if(erreurConnection != -1) {
		std::cout << "Connected to server" << std::endl;
	}
	else {
		std::cout << "No server found" << std::endl;
	}

	return erreurConnection;
}

std::shared_ptr<sfg::Button> buttonSpectate;
std::shared_ptr<sfg::Button> buttonPlayOnline;

void threadFunction() {
	while(CreateSocket() == -1) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	while(ConnectToServer() == -1) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	std::cout << "CONNECTE ! fin du thread" << std::endl;

	buttonPlayOnline->SetState(sfg::Widget::State::ACTIVE);
	buttonSpectate->SetState(sfg::Widget::State::ACTIVE);

	while(true) {
		char matchmakingBuffer[512] = {0};
		recv(serverSocket, matchmakingBuffer, sizeof(matchmakingBuffer), 0);

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

int main() {

#pragma region GUI
	sfg::SFGUI sfgui;

	//auto label = sfg::Label::Create("PUTAIN DE GUI QSLKDJSQKLJ");
	auto buttonPlayOffline = sfg::Button::Create("1v1 local");
	buttonSpectate = sfg::Button::Create("Spectate");
	buttonPlayOnline = sfg::Button::Create("1v1 en ligne");
	auto buttonOptions = sfg::Button::Create("Options");
	auto buttonExit = sfg::Button::Create("Quitter");

	buttonPlayOffline->GetSignal(sfg::Widget::OnLeftClick).Connect(OnButtonPlayOfflineClick);
	buttonSpectate->GetSignal(sfg::Widget::OnLeftClick).Connect(OnButtonSpectateClick);
	buttonPlayOnline->GetSignal(sfg::Widget::OnLeftClick).Connect(OnButtonPlayOnlineClick);
	buttonOptions->GetSignal(sfg::Widget::OnLeftClick).Connect(OnButtonOptionsClick);
	buttonExit->GetSignal(sfg::Widget::OnLeftClick).Connect(OnButtonExitClick);

	buttonPlayOnline->SetState(sfg::Widget::State::INSENSITIVE);
	buttonSpectate->SetState(sfg::Widget::State::INSENSITIVE);

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f);
	//box->Pack(label);
	box->Pack(buttonPlayOnline);
	box->Pack(buttonSpectate);
	box->Pack(buttonPlayOffline);
	box->Pack(buttonOptions);
	box->Pack(buttonExit);

	auto window = sfg::Window::Create();
	window->SetTitle("Menu");
	window->Add(box);
	window->SetRequisition(sf::Vector2f(width / 4, heigth));
#pragma endregion DECLARATION DE LA GUI

	if(InitializeWSA()) {
		std::thread first(threadFunction);
		first.detach();
	}

#pragma region RENDU
	bool isPieceChoose = false;
	render_window.resetGLStates();
	sf::Clock clock;
	while(render_window.isOpen()) {
		// Event processing.
		sf::Event event;
		while(render_window.pollEvent(event)) {
			window->HandleEvent(event);
			switch(event.type) {
			case sf::Event::Closed:
				render_window.close();
				break;
			case sf::Event::MouseButtonPressed:
				if(isGameStarted) {
					if(board->IsInBounds(sf::Mouse::getPosition(render_window))) {
						if(board->CheckSpriteClicked(sf::Mouse::getPosition(render_window), (EColor) team)) {
							isPieceChoose = true;
						}
						else {
							if(isPieceChoose) {
								if(board->AskForMovement(sf::Mouse::getPosition(render_window), (EColor) team)) {
									board->PlayMove();
									if(board->GetWinner() != nullptr) {
										isPieceChoose = false;
										isGameStarted = false;
										if(team == WHITE)
											std::cout << "team : " << "WHITE" << " is winner" << std::endl;
										else
											std::cout << "team : " << "BLACK" << " is winner" << std::endl;
									}
									else {
										isPieceChoose = false;
										team = !team;
									}

								}
							}
						}
					}
				}
				break;
			default:
				break;
			}
		}
		window->Update(clock.restart().asSeconds());
		render_window.clear(sf::Color::Blue);
		if(isGameStarted) {
			drawAll();
			if(isPieceChoose) {
				drawReachablePositionsForSelectedPiece(sf::Mouse::getPosition(render_window), (EColor) team);
			}
		}
		sfgui.Display(render_window);
		render_window.display();
	}
#pragma endregion BOUCLE DE RENDU

	return 0;
}

void drawAll() {
	for(auto sprite : *(board->GetSprites()))
		render_window.draw(*sprite);
}

// TODO: Si possible, ne pas rappeler toutes les frames si pas changé !
void drawReachablePositionsForSelectedPiece(sf::Vector2i position, EColor team) {
	for(auto reachablePositionsForSelectedPiece : *(board->GetReachablePositionsForSelectedPiece(position, (EColor) team))) {
		sf::RectangleShape rectangle(sf::Vector2f(75, 75));
		rectangle.setPosition(sf::Vector2f(200 + (reachablePositionsForSelectedPiece.x * 75), (reachablePositionsForSelectedPiece.y * 75)));
		rectangle.setOutlineColor(sf::Color::Red);
		rectangle.setOutlineThickness(-3);
		rectangle.setFillColor(sf::Color::Transparent);
		render_window.draw(rectangle);
	}
}

void OnButtonPlayOfflineClick() {
	isGameStarted = true;
	board->InitTeam();
	std::cout << "Offline Gaming" << std::endl;
}


void OnButtonSpectateClick() {
	/* Envoie une demande de spec au serveur
	le serveur renvoie l'état de la partie au moment ou le spec arrive
	*/
	std::cout << "Request to the server to spectate a game" << std::endl;
	WriteClient(serverSocket, CreateClientModePacket(serverSocket, SPECTATE).c_str());
}

void OnButtonPlayOnlineClick() {
	/* Demande de connexion au serveur
	tant que le serveur n'a pas envoyé play
	on attend le second joueur
	*/
	std::cout << "Request to the server to play a game" << std::endl;
	WriteClient(serverSocket, CreateClientModePacket(serverSocket, PLAY).c_str());
}

//je me sert du bouton comme bouton pour quitter x)
void OnButtonOptionsClick() {
	isGameStarted = false;
	std::cout << "Options" << std::endl;
}

void OnButtonExitClick() {
	render_window.close();
}

/* Chaque tour le serveur envoie le mouvement de l'adversaire afin d'update l'ui
et le client envoie son mouvement */