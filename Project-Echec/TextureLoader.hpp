#ifndef __TEXTURELOADER_H__
#define __TEXTURELOADER_H__
#include "stdafx.h"

class TextureLoader
{
private:
	sf::Texture* m_whiteRook;
	sf::Texture* m_whiteKnight;
	sf::Texture* m_whiteBishop;
	sf::Texture* m_whiteKing;
	sf::Texture* m_whiteQueen;
	sf::Texture* m_whitePawn;

	sf::Texture* m_blackRook;
	sf::Texture* m_blackKnight;
	sf::Texture* m_blackBishop;
	sf::Texture* m_blackKing;
	sf::Texture* m_blackQueen;
	sf::Texture* m_blackPawn;

	sf::Texture* m_board;
public:
	TextureLoader(bool smooth = true)
	{
		//white
		m_whitePawn = new sf::Texture();
		m_whitePawn->loadFromFile("..\\Ressources\\PawnWhite.png");
		m_whitePawn->setSmooth(smooth);
		
		m_whiteQueen = new sf::Texture();
		m_whiteQueen->loadFromFile("..\\Ressources\\QueenWhite.png");
		m_whiteQueen->setSmooth(smooth);
		
		m_whiteKing = new sf::Texture();
		m_whiteKing->loadFromFile("..\\Ressources\\KingWhite.png");
		m_whiteKing->setSmooth(smooth);
		
		m_whiteBishop = new sf::Texture();
		m_whiteBishop->loadFromFile("..\\Ressources\\BishopWhite.png");
		m_whiteBishop->setSmooth(smooth);
		
		m_whiteKnight = new sf::Texture();
		m_whiteKnight->loadFromFile("..\\Ressources\\KnightWhite.png");
		m_whiteKnight->setSmooth(smooth);
		
		m_whiteRook = new sf::Texture();
		m_whiteRook->loadFromFile("..\\Ressources\\RookWhite.png");
		m_whiteRook->setSmooth(smooth);

		//black
		m_blackPawn = new sf::Texture();
		m_blackPawn->loadFromFile("..\\Ressources\\PawnBlack.png");
		m_blackPawn->setSmooth(smooth);
		
		m_blackQueen = new sf::Texture();
		m_blackQueen->loadFromFile("..\\Ressources\\QueenBlack.png");
		m_blackQueen->setSmooth(smooth);
		
		m_blackKing = new sf::Texture();
		m_blackKing->loadFromFile("..\\Ressources\\KingBlack.png");
		m_blackKing->setSmooth(smooth);
		
		m_blackBishop = new sf::Texture();
		m_blackBishop->loadFromFile("..\\Ressources\\BishopBlack.png");
		m_blackBishop->setSmooth(smooth);
		
		m_blackKnight = new sf::Texture();
		m_blackKnight->loadFromFile("..\\Ressources\\KnightBlack.png");
		m_blackKnight->setSmooth(smooth);
		
		m_blackRook = new sf::Texture();
		m_blackRook->loadFromFile("..\\Ressources\\RookBlack.png");
		m_blackRook->setSmooth(smooth);

		//board
		m_board = new sf::Texture();
		m_board->loadFromFile("..\\Ressources\\damier.png");
		m_board->setSmooth(smooth);

	}
	sf::Texture* GetTexture(ETypePiece type, EColor color) const
	{
		switch (type)
		{
		case ROOK:
			switch (color)
			{
			case WHITE:
				return m_whiteRook;
				break;
			case BLACK:
				return m_blackRook;
				break;
			default:
				break;
			}
			break;
		case KNIGHT:
			switch (color)
			{
			case WHITE:
				return m_whiteKnight;
				break;
			case BLACK:
				return m_blackKnight;
				break;
			default:
				break;
			}
			break;
		case BISHOP:
			switch (color)
			{
			case WHITE:
				return m_whiteBishop;
				break;
			case BLACK:
				return m_blackBishop;
				break;
			default:
				break;
			}
			break;
		case QUEEN:
			switch (color)
			{
			case WHITE:
				return m_whiteQueen;
				break;
			case BLACK:
				return m_blackQueen;
				break;
			default:
				break;
			}
			break;
		case KING:
			switch (color)
			{
			case WHITE:
				return m_whiteKing;
				break;
			case BLACK:
				return m_blackKing;
				break;
			default:
				break;
			}
			break;
		case PAWN:
			switch (color)
			{
			case WHITE:
				return m_whitePawn;
				break;
			case BLACK:
				return m_blackPawn;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	sf::Texture* GetBoard() const { return m_board; }
};

#endif
