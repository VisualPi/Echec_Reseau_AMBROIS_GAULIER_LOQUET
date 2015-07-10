#include <SFML/Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
sf::Texture pawnW, queenW, kingW, bishopW, knightW, rookW, pawnB, queenB, kingB, bishopB, knightB, rookB, damier;
sf::Sprite spPawnW, spQueenW, spKingW, spBishopW, spKnightW, spRookW, spPawnB, spQueenB, spKingB, spBishopB, spKnightB, spRookB, spDamier;
std::vector<sf::Sprite> whitePawn;
std::vector<sf::Sprite> blackPawn;
std::vector<sf::Sprite> whiteRook;
std::vector<sf::Sprite> blackRook;
std::vector<sf::Sprite> whiteKnight;
std::vector<sf::Sprite> blackKnight;
std::vector<sf::Sprite> whiteBishop;
std::vector<sf::Sprite> blackBishop;
sf::Vector2f damierSize = sf::Vector2f(600, 600);


void OnButtonLocalClick();
void OnButtonComputerClick();
void OnButtonLigneClick();
void OnButtonOptionClick();
void OnButtonQuitClick();
void drawAllPieces();

sf::RenderWindow render_window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Hello world!", sf::Style::Titlebar);

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
	window->SetRequisition(sf::Vector2f(SCREEN_WIDTH / 4, SCREEN_HEIGHT));
#pragma endregion DECLARATION DE LA GUI

#pragma region SPRITES
	pawnW.loadFromFile("..\\Ressources\\PawnWhite.png");		pawnW.setSmooth(true);
	queenW.loadFromFile("..\\Ressources\\QueenWhite.png");		queenW.setSmooth(true);
	kingW.loadFromFile("..\\Ressources\\KingWhite.png");		kingW.setSmooth(true);
	bishopW.loadFromFile("..\\Ressources\\BishopWhite.png");	bishopW.setSmooth(true);
	knightW.loadFromFile("..\\Ressources\\KnightWhite.png");	knightW.setSmooth(true);
	rookW.loadFromFile("..\\Ressources\\RookWhite.png");		rookW.setSmooth(true);

	pawnB.loadFromFile("..\\Ressources\\PawnBlack.png");		pawnB.setSmooth(true);
	queenB.loadFromFile("..\\Ressources\\QueenBlack.png");		queenB.setSmooth(true);
	kingB.loadFromFile("..\\Ressources\\KingBlack.png");		kingB.setSmooth(true);
	bishopB.loadFromFile("..\\Ressources\\BishopBlack.png");	bishopB.setSmooth(true);
	knightB.loadFromFile("..\\Ressources\\KnightBlack.png");	knightB.setSmooth(true);
	damier.loadFromFile("..\\Ressources\\damier.png");			damier.setSmooth(true);
	rookB.loadFromFile("..\\Ressources\\RookBlack.png");		rookB.setSmooth(true);

	spPawnW.setTexture(pawnW);		spPawnW.setScale(sf::Vector2f(.3125f, .3125f));
	spQueenW.setTexture(queenW);	spQueenW.setScale(sf::Vector2f(.3125f, .3125f));
	spKingW.setTexture(kingW);		spKingW.setScale(sf::Vector2f(.3125f, .3125f));
	spBishopW.setTexture(bishopW);	spBishopW.setScale(sf::Vector2f(.3125f, .3125f));
	spKnightW.setTexture(knightW);	spKnightW.setScale(sf::Vector2f(.3125f, .3125f));
	spRookW.setTexture(rookW);		spRookW.setScale(sf::Vector2f(.3125f, .3125f));
	spPawnB.setTexture(pawnB);		spPawnB.setScale(sf::Vector2f(.3125f, .3125f));
	spQueenB.setTexture(queenB);	spQueenB.setScale(sf::Vector2f(.3125f, .3125f));
	spKingB.setTexture(kingB);		spKingB.setScale(sf::Vector2f(.3125f, .3125f));
	spBishopB.setTexture(bishopB);	spBishopB.setScale(sf::Vector2f(.3125f, .3125f));
	spKnightB.setTexture(knightB);	spKnightB.setScale(sf::Vector2f(.3125f, .3125f));
	spRookB.setTexture(rookB);		spRookB.setScale(sf::Vector2f(.3125f, .3125f));

	spDamier.setTexture(damier);	spDamier.setScale(sf::Vector2f(.5859375f, .5859375f)); // damier de 1024*1024 dans 600*600 donc x et y = 600/1024

	spDamier.setPosition(SCREEN_WIDTH - damierSize.x, 0);
	
	for (int i = 0; i < 8; ++i)
	{
		whitePawn.push_back(sf::Sprite(spPawnW));
		blackPawn.push_back(sf::Sprite(spPawnB));
	}
	for (int i = 0; i < 2; ++i)
	{
		whiteRook.push_back(sf::Sprite(spRookW));
		blackRook.push_back(sf::Sprite(spRookB));
		whiteKnight.push_back(sf::Sprite(spKnightW));
		blackKnight.push_back(sf::Sprite(spKnightB));
		whiteBishop.push_back(sf::Sprite(spBishopW));
		blackBishop.push_back(sf::Sprite(spBishopB));
	}

#pragma endregion DECLARATION DES TEXTURES ET SPRITES



#pragma region RENDU
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
			default:
				break;
			}
		}
		window->Update(clock.restart().asSeconds());
		render_window.clear(sf::Color::Blue);
		render_window.draw(spDamier);
		drawAllPieces();
		sfgui.Display(render_window);
		render_window.display();
	}
#pragma endregion BOUCLE DE RENDU

	return 0;
}

void drawAllPieces()
{
	render_window.draw(blackRook.at(0));
	render_window.draw(blackRook.at(1));

	render_window.draw(blackKnight.at(0));
	render_window.draw(blackKnight.at(1));

	render_window.draw(blackBishop.at(0));
	render_window.draw(blackBishop.at(1));

	render_window.draw(spQueenB);
	render_window.draw(spKingB);

	for (int i = 0; i < 8; ++i)
	{
		render_window.draw(blackPawn.at(i));
	}
	//-----------------------------------------//
	render_window.draw(whiteRook.at(0));
	render_window.draw(whiteRook.at(1));

	render_window.draw(whiteKnight.at(0));
	render_window.draw(whiteKnight.at(1));

	render_window.draw(whiteBishop.at(0));
	render_window.draw(whiteBishop.at(1));

	render_window.draw(spQueenW);
	render_window.draw(spKingW);

	for (int i = 0; i < 8; ++i)
	{
		render_window.draw(whitePawn.at(i));
	}
}
void preparePlateau()
{
	blackRook.at(0).setPosition(200, 0);
	blackRook.at(1).setPosition(725, 0);

	blackKnight.at(0).setPosition(275, 0);
	blackKnight.at(1).setPosition(650, 0);

	blackBishop.at(0).setPosition(350, 0);
	blackBishop.at(1).setPosition(575, 0);

	spKingB.setPosition(425,0);
	spQueenB.setPosition(500,0);

	for (int i = 0; i < 8; ++i)
	{
		blackPawn.at(i).setPosition(200 + (i * 75),75);
	}
	//--------------------------------------------------//
	whiteRook.at(0).setPosition(200, damierSize.y - 75);
	whiteRook.at(1).setPosition(725, damierSize.y - 75);

	whiteKnight.at(0).setPosition(275, damierSize.y - 75);
	whiteKnight.at(1).setPosition(650, damierSize.y - 75);

	whiteBishop.at(0).setPosition(350, damierSize.y - 75);
	whiteBishop.at(1).setPosition(575, damierSize.y - 75);

	spKingW.setPosition(425, damierSize.y - 75);
	spQueenW.setPosition(500, damierSize.y - 75);

	for (int i = 0; i < 8; ++i)
	{
		whitePawn.at(i).setPosition(200 + (i * 75), damierSize.y - 150);
	}
}
void OnButtonLocalClick()
{
	preparePlateau();
	std::cout << "Solo game " << std::endl;
}
void OnButtonComputerClick()
{
	preparePlateau();
	std::cout << "Computer game " << std::endl;

}
void OnButtonLigneClick()
{
	preparePlateau();
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

