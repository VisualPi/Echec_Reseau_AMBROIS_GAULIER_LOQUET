#ifndef __PIECE_H__
#define __PIECE_H__
#include "stdafx.h"


class Piece
{
private:
	sf::Sprite*		m_sprite;
	ETypePiece		m_type;
	sf::Color		m_defaultColor;
	int				m_lap;
	sf::Vector2f*	m_case;
	EColor			m_color;

public:
	Piece(sf::Texture* texture, ETypePiece type, EColor color) : m_color(color), m_lap(1)
	{
		this->m_sprite = new sf::Sprite();
		this->m_sprite->setTexture(*texture);
		this->m_defaultColor = this->m_sprite->getColor();
		this->m_sprite->setScale(sf::Vector2f(.3125f, .3125f)); //TODO : a generer avec parametre de la map + param sprite
		this->m_type = type;
	}
	sf::Sprite*		GetSprite() const { return m_sprite; }
	ETypePiece		GetType()	const { return m_type; }
	sf::Color		GetDefaultColor() const { return m_defaultColor; }
	int				GetLap() const { return m_lap; }
	void			setLap(const int value) { m_lap = value; }
	sf::Vector2f	GetCase() const { return *m_case; }
	void			SetCase(int x, int y) { m_case = new sf::Vector2f(x, y); }
	EColor			GetColor() const { return m_color; }
	void			SetColor(EColor color) { m_color = color; }
	
	void			SetPosition(sf::Vector2f position, int indiceX, int indiceY)
	{
		this->m_sprite->setPosition(position);
		this->m_case = new sf::Vector2f(indiceX, indiceY);

	}

};

#endif
