#pragma once

#include "Utils.h"

class Piece
{
private:
	ETypePiece		m_type;
	int				m_lap;
	Vector2f*		m_case;
	EColor			m_color;

public:
	Piece(ETypePiece type, EColor color) : m_color(color), m_lap(1)
	{
		this->m_type = type;
	}

	ETypePiece		GetType()	const { return m_type; }
	int				GetLap() const { return m_lap; }
	void			setLap(const int value) { m_lap = value; }
	Vector2f		GetCase() const { return *m_case; }
	void			SetCase(int x, int y) { m_case = new Vector2f(x, y); }
	EColor			GetColor() const { return m_color; }
	void			SetColor(EColor color) { m_color = color; }
	
	void			SetPosition(int indiceX, int indiceY)
	{
		this->m_case = new Vector2f(indiceX, indiceY);
	}
};