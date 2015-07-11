#ifndef __PIECE_H__
#define __PIECE_H__
#include "stdafx.h"
enum ETypePiece{ ROOK, KNIGHT, BISHOP, QUEEN, KING, PAWN};

class Piece
{
private:
	sf::Sprite* m_sprite;
	ETypePiece	m_type;
	sf::Color	m_defaultColor;
	int			m_lap;

public:
	Piece(sf::Texture* texture, ETypePiece type) : m_lap(1)
	{
		this->m_sprite = new sf::Sprite();
		this->m_sprite->setTexture(*texture);
		this->m_defaultColor = this->m_sprite->getColor();
		this->m_sprite->setScale(sf::Vector2f(.3125f, .3125f)); //TODO : a generer avec parametre de la map + param sprite
		this->m_type = type;
	}
	sf::Sprite* GetSprite() const { return m_sprite; }
	ETypePiece	GetType()	const { return m_type; }
	sf::Color	GetDefaultColor() const { return m_defaultColor; }
	int			GetLap() const { return m_lap; }
	void		setLap(const int value) { m_lap = value; }
};

#endif
