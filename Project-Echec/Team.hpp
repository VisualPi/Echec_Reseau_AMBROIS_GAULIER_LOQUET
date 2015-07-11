#ifndef __TEAM_H__
#define __TEAM_H__
#include "stdafx.h"
enum EColor{ WHITE, BLACK};

class Team
{
private:
	std::vector<Piece*> m_rooks;
	std::vector<Piece*> m_knights;
	std::vector<Piece*> m_bishops;
	std::vector<Piece*> m_pawns;
	Piece*				m_king;
	Piece*				m_queen;
	EColor				m_color;

public:
	Team(sf::Texture* rook, sf::Texture* knight, sf::Texture* bishop, sf::Texture* pawn, sf::Texture* king, sf::Texture* queen, EColor color)
	{
		for (int i = 0; i < 2; ++i)
			this->m_rooks.push_back(new Piece(rook, ETypePiece::ROOK));
		for (int i = 0; i < 2; ++i)
			this->m_knights.push_back(new Piece(knight, ETypePiece::KNIGHT));
		for (int i = 0; i < 2; ++i)
			this->m_bishops.push_back(new Piece(bishop, ETypePiece::BISHOP));
		for (int i = 0; i < 8; ++i)
			this->m_pawns.push_back(new Piece(pawn, ETypePiece::PAWN));
		m_king = new Piece(king, ETypePiece::KING);
		m_queen = new Piece(queen, ETypePiece::QUEEN);
	}
	void Init(sf::Vector2f boardSize[8][8], EColor color)
	{
		switch (color)
		{
		case WHITE:
			m_rooks.at(0)->GetSprite()->setPosition(boardSize[7][0]);
			m_rooks.at(1)->GetSprite()->setPosition(boardSize[7][7]);

			m_knights.at(0)->GetSprite()->setPosition(boardSize[7][1]);
			m_knights.at(1)->GetSprite()->setPosition(boardSize[7][6]);

			m_bishops.at(0)->GetSprite()->setPosition(boardSize[7][2]);
			m_bishops.at(1)->GetSprite()->setPosition(boardSize[7][5]);

			m_king->GetSprite()->setPosition(boardSize[7][4]);
			m_queen->GetSprite()->setPosition(boardSize[7][3]);

			for (int i = 0; i < 8; ++i)
				m_pawns.at(i)->GetSprite()->setPosition(boardSize[6][i]);
			break;
		case BLACK:
			m_rooks.at(0)->GetSprite()->setPosition(boardSize[0][0]);
			m_rooks.at(1)->GetSprite()->setPosition(boardSize[0][7]);

			m_knights.at(0)->GetSprite()->setPosition(boardSize[0][1]);
			m_knights.at(1)->GetSprite()->setPosition(boardSize[0][6]);

			m_bishops.at(0)->GetSprite()->setPosition(boardSize[0][2]);
			m_bishops.at(1)->GetSprite()->setPosition(boardSize[0][5]);

			m_king->GetSprite()->setPosition(boardSize[0][3]);
			m_queen->GetSprite()->setPosition(boardSize[0][4]);

			for (int i = 0; i < 8; ++i)
				m_pawns.at(i)->GetSprite()->setPosition(boardSize[1][i]);
			break;
		default:
			break;
		}		
	}
	std::vector<Piece*>* GetPieces() const
	{
		std::vector<Piece*>* vRet = new std::vector<Piece*>();
		for (int i = 0; i < 2; ++i)
			vRet->push_back(m_rooks.at(i));
		for (int i = 0; i < 2; ++i)
			vRet->push_back(m_knights.at(i));
		for (int i = 0; i < 2; ++i)
			vRet->push_back(m_bishops.at(i));
		for (int i = 0; i < 8; ++i)
			vRet->push_back(m_pawns.at(i));
		vRet->push_back(m_king);
		vRet->push_back(m_queen);
		return vRet;
	}
};

#endif
