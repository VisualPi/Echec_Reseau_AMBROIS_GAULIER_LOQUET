#ifndef __TEAM_H__
#define __TEAM_H__
#include "stdafx.h"


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
	Team(sf::Texture* rook, sf::Texture* knight, sf::Texture* bishop, sf::Texture* pawn, sf::Texture* king, sf::Texture* queen, EColor color) : m_color(color)
	{
		for (int i = 0; i < 2; ++i)
			this->m_rooks.push_back(new Piece(rook, ETypePiece::ROOK, color));
		for (int i = 0; i < 2; ++i)
			this->m_knights.push_back(new Piece(knight, ETypePiece::KNIGHT, color));
		for (int i = 0; i < 2; ++i)
			this->m_bishops.push_back(new Piece(bishop, ETypePiece::BISHOP, color));
		for (int i = 0; i < 8; ++i)
			this->m_pawns.push_back(new Piece(pawn, ETypePiece::PAWN, color));
		m_king = new Piece(king, ETypePiece::KING, color);
		m_queen = new Piece(queen, ETypePiece::QUEEN, color);
	}

	void Init(sf::Vector2f boardSize[8][8])
	{
		switch (m_color)
		{
		case WHITE:
			m_rooks.at(0)->SetPosition(boardSize[0][7], 0, 7);
			m_rooks.at(1)->SetPosition(boardSize[7][7], 7, 7);

			m_knights.at(0)->SetPosition(boardSize[1][7], 1, 7);
			m_knights.at(1)->SetPosition(boardSize[6][7], 6, 7);

			m_bishops.at(0)->SetPosition(boardSize[2][7], 2, 7);
			m_bishops.at(1)->SetPosition(boardSize[5][7], 5, 7);

			m_king->SetPosition(boardSize[4][7], 4, 7);
			m_queen->SetPosition(boardSize[3][7], 3, 7);

			for (int i = 0; i < 8; ++i)
			{
				m_pawns.at(i)->SetPosition(boardSize[i][6], i, 6);
			}
			break;
		case BLACK:
			m_rooks.at(0)->SetPosition(boardSize[0][0], 0, 0);
			m_rooks.at(1)->SetPosition(boardSize[7][0], 7, 0);

			m_knights.at(0)->SetPosition(boardSize[1][0], 1, 0);
			m_knights.at(1)->SetPosition(boardSize[6][0], 6, 0);

			m_bishops.at(0)->SetPosition(boardSize[2][0], 2, 0);
			m_bishops.at(1)->SetPosition(boardSize[5][0], 5, 0);

			m_king->SetPosition(boardSize[4][0], 4, 0);
			m_queen->SetPosition(boardSize[3][0], 3, 0);

			for (int i = 0; i < 8; ++i)
			{
				m_pawns.at(i)->SetPosition(boardSize[i][1], i, 1);
			}
			break;
		default:
			break;
		}		
	}
	std::vector<Piece*>* GetPieces() const
	{
		std::vector<Piece*>* vRet = new std::vector<Piece*>();
		for (int i = 0; i < m_rooks.size(); ++i)
			vRet->push_back(m_rooks.at(i));
		for (int i = 0; i < m_knights.size(); ++i)
			vRet->push_back(m_knights.at(i));
		for (int i = 0; i < m_bishops.size(); ++i)
			vRet->push_back(m_bishops.at(i));
		for (int i = 0; i < m_pawns.size(); ++i)
			vRet->push_back(m_pawns.at(i));
		if(m_king != nullptr)	 vRet->push_back(m_king);
		if (m_queen != nullptr)  vRet->push_back(m_queen);
		return vRet;
	}

	EColor* KillPiece(Piece* piece)
	{
		switch (piece->GetType())
		{
		case ROOK:
			for (auto it = m_rooks.begin(); it != m_rooks.end(); ++it)
			{
				if (*it == piece)
				{
					m_rooks.erase(it);
					break;
				}
			}
			break;
		case KNIGHT:
			for (auto it = m_knights.begin(); it != m_knights.end(); ++it)
			{
				if (*it == piece)
				{
					m_knights.erase(it);
					break;
				}
			}
			break;
		case BISHOP:
			for (auto it = m_bishops.begin(); it != m_bishops.end(); ++it)
			{
				if (*it == piece)
				{
					m_bishops.erase(it);
					break;
				}
			}
			break;
		case QUEEN:
			m_queen = nullptr;
			break;
		case KING:
			m_king = nullptr; //WIN en meme temps ..
			return piece->GetColor() == WHITE ? new EColor(BLACK) : new EColor(WHITE);
			break;
		case PAWN:
			for (auto it = m_pawns.begin(); it != m_pawns.end(); ++it)
			{
				if (*it == piece)
				{
					m_pawns.erase(it);
					break;
				}
			}
			break;
		default:
			break;
		}
		
		return nullptr;
	}
};

#endif
