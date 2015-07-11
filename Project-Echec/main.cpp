#include "stdafx.h"


const int width		= 800;
const int heigth	= 600;
bool isGameStarted = false;

void OnButtonLocalClick();
void OnButtonComputerClick();
void OnButtonLigneClick();
void OnButtonOptionClick();
void OnButtonQuitClick();
void drawAll();

sf::RenderWindow render_window(sf::VideoMode(width, heigth), "Hello world!", sf::Style::Titlebar); //JE SAIS PAS POURQUOI VIDEOMODE ACCEPETE PAS LES MACCROS
ChessBoard* board = new ChessBoard(width, heigth);

//EColor currentTeam = WHITE;
bool team = false; //WHITE

int main()
{

#pragma region GUI
	sfg::SFGUI sfgui;

	//auto label = sfg::Label::Create("PUTAIN DE GUI QSLKDJSQKLJ");
	auto buttonLocal = sfg::Button::Create("1v1 local");
	auto buttonLigne = sfg::Button::Create("1v1 en ligne");
	auto buttonComputer = sfg::Button::Create("1 vs computer");
	auto buttonOption = sfg::Button::Create("Options");
	auto buttonQuitter = sfg::Button::Create("Quitter");

	buttonLocal->GetSignal(sfg::Widget::OnLeftClick).Connect(OnButtonLocalClick);
	buttonComputer->GetSignal(sfg::Widget::OnLeftClick).Connect(OnButtonComputerClick);
	buttonLigne->GetSignal(sfg::Widget::OnLeftClick).Connect(OnButtonLigneClick);
	buttonOption->GetSignal(sfg::Widget::OnLeftClick).Connect(OnButtonOptionClick);
	buttonQuitter->GetSignal(sfg::Widget::OnLeftClick).Connect(OnButtonQuitClick);

	auto box = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.0f);
	//box->Pack(label);
	box->Pack(buttonLigne);
	box->Pack(buttonLocal);
	box->Pack(buttonComputer);
	box->Pack(buttonOption);
	box->Pack(buttonQuitter);
	//box->Pack(button, false);

	auto window = sfg::Window::Create();
	window->SetTitle("Menu");
	window->Add(box);
	window->SetRequisition(sf::Vector2f(width / 4, heigth));
#pragma endregion DECLARATION DE LA GUI

#pragma region RENDU
	bool isPieceChoose = false;
	render_window.resetGLStates();
	sf::Clock clock;
	while (render_window.isOpen())
	{
		// Event processing.
		sf::Event event;
		while (render_window.pollEvent(event))
		{
			window->HandleEvent(event);
			switch (event.type)
			{
			case sf::Event::Closed:
				render_window.close();
				break;
			case sf::Event::MouseButtonPressed :
				if (isGameStarted)
				{
					if (board->IsInBounds(sf::Mouse::getPosition(render_window)))
					{
						if (board->CheckSpriteClicked(sf::Mouse::getPosition(render_window), (EColor)team))
						{
							isPieceChoose = true;
						}
						else
						{
							if (isPieceChoose)
							{
								if (board->AskForMovement(sf::Mouse::getPosition(render_window), (EColor)team))
								{
									isPieceChoose = false;
									team = !team;
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
		if(isGameStarted)
			drawAll();
		sfgui.Display(render_window);
		render_window.display();
	}
#pragma endregion BOUCLE DE RENDU

	return 0;
}

void drawAll()
{
	for (auto sprite : *(board->GetSprites()))
		render_window.draw(*sprite);
}

void OnButtonLocalClick()
{
	isGameStarted = true;
	board->InitTeam();
	std::cout << "Solo game " << std::endl;
}
void OnButtonComputerClick()
{
	isGameStarted = true;
	board->InitTeam();
	std::cout << "Computer game " << std::endl;

}
void OnButtonLigneClick()
{
	isGameStarted = true;
	board->InitTeam();
	std::cout << "Online Game" << std::endl;
}
void OnButtonOptionClick()
{
	std::cout << "Options" << std::endl;
}
void OnButtonQuitClick()
{
	render_window.close();
}

