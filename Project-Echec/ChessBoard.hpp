#ifndef __CHESSBOARD_H__
#define __CHESSBOARD_H__
#include "stdafx.h"

class ChessBoard
{
private:
	TextureLoader*	m_textures;
	sf::Sprite*		m_sprite;
	Team*			m_teamWhite;
	Team*			m_teamBlack;
	Piece*			m_currentPiece;
	sf::Vector2f	m_boardSize;
	sf::Vector2f	m_quadrillage[8][8];

	sf::Sound*		m_moveSound;
	EColor*			m_winner;

public:
	ChessBoard(int screenWidth, int screenHeight)
	{
		m_textures = new TextureLoader();
		this->m_sprite = new sf::Sprite();
		this->m_sprite->setTexture(*(m_textures->GetBoard()));
		this->m_sprite->setScale(sf::Vector2f(.5859375f, .5859375f)); //TODO : a generer avec parametre de la map + param sprite
		this->m_sprite->setPosition(screenWidth - this->m_sprite->getLocalBounds().width * this->m_sprite->getScale().x, 0);

		m_boardSize = sf::Vector2f(this->m_sprite->getLocalBounds().width * this->m_sprite->getScale().x,
			this->m_sprite->getLocalBounds().height * this->m_sprite->getScale().y);
		int uiX = screenWidth - m_boardSize.x;

		for (int y = 0; y < 8; ++y)
		{
			for (int x = 0; x < 8; ++x)
			{
				m_quadrillage[x][y] = sf::Vector2f(uiX + (x * (m_boardSize.x / 8)), y * (m_boardSize.y / 8));
				//std::cout << "[" << m_quadrillage[x][y].x << "," << m_quadrillage[x][y].y << "]";
			}
			//std::cout << std::endl;
		}
		m_moveSound = new sf::Sound();
		m_moveSound->setBuffer(*(m_textures->GetSound()));
	}

	void InitTeam()
	{
		m_teamWhite = new Team(m_textures->GetTexture(ROOK, WHITE),
			m_textures->GetTexture(KNIGHT, WHITE),
			m_textures->GetTexture(BISHOP, WHITE),
			m_textures->GetTexture(PAWN, WHITE),
			m_textures->GetTexture(KING, WHITE),
			m_textures->GetTexture(QUEEN, WHITE),
			WHITE
			);
		m_teamBlack = new Team(m_textures->GetTexture(ROOK, BLACK),
			m_textures->GetTexture(KNIGHT, BLACK),
			m_textures->GetTexture(BISHOP, BLACK),
			m_textures->GetTexture(PAWN, BLACK),
			m_textures->GetTexture(KING, BLACK),
			m_textures->GetTexture(QUEEN, BLACK),
			BLACK
			);

		m_teamWhite->Init(m_quadrillage);
		m_teamBlack->Init(m_quadrillage);
		m_winner = nullptr;

	}
	std::vector<sf::Sprite*>* GetSprites() const
	{
		std::vector<sf::Sprite*>* vRet = new std::vector<sf::Sprite*>();
		vRet->push_back(m_sprite);
		for (auto piece : *(m_teamWhite->GetPieces()))
			vRet->push_back(piece->GetSprite());
		for (auto piece : *(m_teamBlack->GetPieces()))
			vRet->push_back(piece->GetSprite());
		return vRet;
	}

	bool IsInBounds(sf::Vector2i position)
	{
		return (position.x > m_quadrillage[0][0].x && position.x < m_quadrillage[7][7].x + (m_boardSize.x / 8) //+ la taille d'une case
			&& position.y > m_quadrillage[0][0].y && position.y < m_quadrillage[7][7].y + (m_boardSize.y / 8));
	}
	bool CheckSpriteClicked(sf::Vector2i position, EColor team)
	{
		bool bRet = false;
		switch (team)
		{
		case WHITE:
			for (auto piece : *(m_teamWhite->GetPieces()))
			{
				if (position.x > piece->GetSprite()->getPosition().x
					&& position.x < piece->GetSprite()->getPosition().x + (piece->GetSprite()->getLocalBounds().width * piece->GetSprite()->getScale().x)
					&& position.y > piece->GetSprite()->getPosition().y
					&& position.y < piece->GetSprite()->getPosition().y + (piece->GetSprite()->getLocalBounds().height * piece->GetSprite()->getScale().y)
					)
				{
					m_currentPiece = piece;
					piece->GetSprite()->setColor(sf::Color::Blue);
					bRet = true;

				}
				else
					piece->GetSprite()->setColor(piece->GetDefaultColor());
			}
			break;
		case BLACK:
			for (auto piece : *(m_teamBlack->GetPieces()))
			{
				if (position.x > piece->GetSprite()->getPosition().x
					&& position.x < piece->GetSprite()->getPosition().x + (piece->GetSprite()->getLocalBounds().width * piece->GetSprite()->getScale().x)
					&& position.y > piece->GetSprite()->getPosition().y
					&& position.y < piece->GetSprite()->getPosition().y + (piece->GetSprite()->getLocalBounds().height * piece->GetSprite()->getScale().y)
					)
				{
					m_currentPiece = piece;
					piece->GetSprite()->setColor(sf::Color::Blue);
					bRet = true;
				}
				else
					piece->GetSprite()->setColor(piece->GetDefaultColor());
			}
			break;
		default:
			break;
		}
		return bRet;
	}

	bool AskForMovement(sf::Vector2i position, EColor team)
	{
		sf::Vector2f targetsquare = GetCase(position);
		sf::Vector2f currSquare = GetCase(m_currentPiece->GetSprite()->getPosition());
		bool bRet = false;
		
		switch (m_currentPiece->GetType())
		{
		case ROOK:
			if (targetsquare.y != currSquare.y && targetsquare.x == currSquare.x) //VERTICAL
			{
				auto piece = GetPieceAtCase(targetsquare);
				if ((piece != nullptr && piece->GetColor() != team) || piece == nullptr) //s'il y a quelqu'un de la team adverse ou personne
				{
					bRet = true; // setter a defaut a true ici
					if (targetsquare.y > currSquare.y)
					{
						for (int i = currSquare.y + 1; i < targetsquare.y; ++i)
						{
							if (GetPieceAtCase(sf::Vector2f(currSquare.x, i)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else
					{
						for (int i = currSquare.y - 1; i > targetsquare.y; --i)
						{
							if (GetPieceAtCase(sf::Vector2f(currSquare.x, i)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					if (bRet)
					{
						m_currentPiece->SetPosition(m_quadrillage[(int)targetsquare.x][(int)targetsquare.y], targetsquare.x, targetsquare.y);
						if (piece != nullptr)
						{
							if (team == WHITE)
								m_winner = m_teamBlack->KillPiece(piece);
							else
								m_winner = m_teamWhite->KillPiece(piece);
						}
							
					}
				}
				else
					bRet = false;
			}
			else if (targetsquare.y == currSquare.y && targetsquare.x != currSquare.x)//HORIZONTAL
			{
				auto piece = GetPieceAtCase(targetsquare);
				if ((piece != nullptr && piece->GetColor() != team) || piece == nullptr) //s'il y a quelqu'un de la team adverse ou personne
				{
					bRet = true; // setter a defaut a true ici
					if (targetsquare.x > currSquare.x)
					{
						for (int i = currSquare.x + 1; i < targetsquare.x; ++i)
						{
							if (GetPieceAtCase(sf::Vector2f(i, currSquare.y)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else
					{
						for (int i = currSquare.x - 1; i > targetsquare.x; --i)
						{
							if (GetPieceAtCase(sf::Vector2f(i, currSquare.y)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					if (bRet)
					{
						m_currentPiece->SetPosition(m_quadrillage[(int)targetsquare.x][(int)targetsquare.y], targetsquare.x, targetsquare.y);
						if (piece != nullptr)
						{
							if (team == WHITE)
								m_winner = m_teamBlack->KillPiece(piece);
							else
								m_winner = m_teamWhite->KillPiece(piece);
						}

					}
				}
				else
					bRet = false;
			}
			break;
		case KNIGHT:
			if (targetsquare.x == currSquare.x - 1 && targetsquare.y == currSquare.y - 2
				|| targetsquare.x == currSquare.x + 1 && targetsquare.y == currSquare.y - 2
				|| targetsquare.x == currSquare.x - 1 && targetsquare.y == currSquare.y + 2
				|| targetsquare.x == currSquare.x + 1 && targetsquare.y == currSquare.y + 2
				|| targetsquare.x == currSquare.x + 2 && targetsquare.y == currSquare.y - 1
				|| targetsquare.x == currSquare.x + 2 && targetsquare.y == currSquare.y + 1
				|| targetsquare.x == currSquare.x - 2 && targetsquare.y == currSquare.y - 1
				|| targetsquare.x == currSquare.x - 2 && targetsquare.y == currSquare.y + 1
				)
			{
				auto piece = GetPieceAtCase(targetsquare);
				if (piece != nullptr)
				{
					if (team == WHITE)
						m_winner = m_teamBlack->KillPiece(piece);
					else
						m_winner = m_teamWhite->KillPiece(piece);
				}
				m_currentPiece->SetPosition(m_quadrillage[(int)targetsquare.x][(int)targetsquare.y], targetsquare.x, targetsquare.y);
				bRet = true;
			}
			break;
		case BISHOP:
			if ((targetsquare.y != currSquare.y && targetsquare.x != currSquare.x) 
				&& (abs(targetsquare.y - currSquare.y) == abs(targetsquare.x - currSquare.x))) //VERTICAL
			{
				auto piece = GetPieceAtCase(targetsquare);
				if ((piece != nullptr && piece->GetColor() != team) || piece == nullptr) //s'il y a quelqu'un de la team adverse ou personne
				{
					bRet = true; // setter a defaut a true ici
					if (targetsquare.y > currSquare.y && targetsquare.x > currSquare.x)
					{
						for (int i = currSquare.x + 1, j = currSquare.y + 1; i < targetsquare.x, j < targetsquare.y; ++i, ++j)
						{
							if (GetPieceAtCase(sf::Vector2f(i, j)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else if (targetsquare.y < currSquare.y && targetsquare.x < currSquare.x)
					{
						for (int i = currSquare.x - 1, j = currSquare.y - 1; i > targetsquare.x, j > targetsquare.y; --i, --j)
						{
							if (GetPieceAtCase(sf::Vector2f(i, j)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else if (targetsquare.y < currSquare.y && targetsquare.x > currSquare.x)
					{
						for (int i = currSquare.x + 1, j = currSquare.y - 1; i < targetsquare.x, j > targetsquare.y; ++i, --j)
						{
							if (GetPieceAtCase(sf::Vector2f(i, j)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else if (targetsquare.y > currSquare.y && targetsquare.x < currSquare.x)
					{
						for (int i = currSquare.x - 1, j = currSquare.y + 1; i > targetsquare.x, j < targetsquare.y; --i, ++j)
						{
							if (GetPieceAtCase(sf::Vector2f(i, j)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					if (bRet)
					{
						m_currentPiece->SetPosition(m_quadrillage[(int)targetsquare.x][(int)targetsquare.y], targetsquare.x, targetsquare.y);
						if (piece != nullptr)
						{
							if (team == WHITE)
								m_winner = m_teamBlack->KillPiece(piece);
							else
								m_winner = m_teamWhite->KillPiece(piece);
						}

					}
				}
				else
					bRet = false;
			}
			break;
		case QUEEN: //ROOK + BISHOP
			if (targetsquare.y != currSquare.y && targetsquare.x == currSquare.x) //VERTICAL
			{
				auto piece = GetPieceAtCase(targetsquare);
				if ((piece != nullptr && piece->GetColor() != team) || piece == nullptr) //s'il y a quelqu'un de la team adverse ou personne
				{
					bRet = true; // setter a defaut a true ici
					if (targetsquare.y > currSquare.y)
					{
						for (int i = currSquare.y + 1; i < targetsquare.y; ++i)
						{
							if (GetPieceAtCase(sf::Vector2f(currSquare.x, i)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else
					{
						for (int i = currSquare.y - 1; i > targetsquare.y; --i)
						{
							if (GetPieceAtCase(sf::Vector2f(currSquare.x, i)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					if (bRet)
					{
						m_currentPiece->SetPosition(m_quadrillage[(int)targetsquare.x][(int)targetsquare.y], targetsquare.x, targetsquare.y);
						if (piece != nullptr)
						{
							if (team == WHITE)
								m_winner = m_teamBlack->KillPiece(piece);
							else
								m_winner = m_teamWhite->KillPiece(piece);
						}

					}
				}
				else
					bRet = false;
			}
			else if (targetsquare.y == currSquare.y && targetsquare.x != currSquare.x)//HORIZONTAL
			{
				auto piece = GetPieceAtCase(targetsquare);
				if ((piece != nullptr && piece->GetColor() != team) || piece == nullptr) //s'il y a quelqu'un de la team adverse ou personne
				{
					bRet = true; // setter a defaut a true ici
					if (targetsquare.x > currSquare.x)
					{
						for (int i = currSquare.x + 1; i < targetsquare.x; ++i)
						{
							if (GetPieceAtCase(sf::Vector2f(i, currSquare.y)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else
					{
						for (int i = currSquare.x - 1; i > targetsquare.x; --i)
						{
							if (GetPieceAtCase(sf::Vector2f(i, currSquare.y)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					if (bRet)
					{
						m_currentPiece->SetPosition(m_quadrillage[(int)targetsquare.x][(int)targetsquare.y], targetsquare.x, targetsquare.y);
						if (piece != nullptr)
						{
							if (team == WHITE)
								m_winner = m_teamBlack->KillPiece(piece);
							else
								m_winner = m_teamWhite->KillPiece(piece);
						}

					}
				}
				else
					bRet = false;
			}
			else if ((targetsquare.y != currSquare.y && targetsquare.x != currSquare.x)
				&& (abs(targetsquare.y - currSquare.y) == abs(targetsquare.x - currSquare.x))) //VERTICAL
			{
				auto piece = GetPieceAtCase(targetsquare);
				if ((piece != nullptr && piece->GetColor() != team) || piece == nullptr) //s'il y a quelqu'un de la team adverse ou personne
				{
					bRet = true; // setter a defaut a true ici
					if (targetsquare.y > currSquare.y && targetsquare.x > currSquare.x)
					{
						for (int i = currSquare.x + 1, j = currSquare.y + 1; i < targetsquare.x, j < targetsquare.y; ++i, ++j)
						{
							if (GetPieceAtCase(sf::Vector2f(i, j)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else if (targetsquare.y < currSquare.y && targetsquare.x < currSquare.x)
					{
						for (int i = currSquare.x - 1, j = currSquare.y - 1; i > targetsquare.x, j > targetsquare.y; --i, --j)
						{
							if (GetPieceAtCase(sf::Vector2f(i, j)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else if (targetsquare.y < currSquare.y && targetsquare.x > currSquare.x)
					{
						for (int i = currSquare.x + 1, j = currSquare.y - 1; i < targetsquare.x, j > targetsquare.y; ++i, --j)
						{
							if (GetPieceAtCase(sf::Vector2f(i, j)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else if (targetsquare.y > currSquare.y && targetsquare.x < currSquare.x)
					{
						for (int i = currSquare.x - 1, j = currSquare.y + 1; i > targetsquare.x, j < targetsquare.y; --i, ++j)
						{
							if (GetPieceAtCase(sf::Vector2f(i, j)) != nullptr)
							{
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					if (bRet)
					{
						m_currentPiece->SetPosition(m_quadrillage[(int)targetsquare.x][(int)targetsquare.y], targetsquare.x, targetsquare.y);
						if (piece != nullptr)
						{
							if (team == WHITE)
								m_winner = m_teamBlack->KillPiece(piece);
							else
								m_winner = m_teamWhite->KillPiece(piece);
						}

					}
				}
				else
					bRet = false;
			}
			break;
		case KING:
			if (targetsquare.x == currSquare.x + 1 && targetsquare.y == currSquare.y
				|| targetsquare.x == currSquare.x - 1 && targetsquare.y == currSquare.y
				|| targetsquare.x == currSquare.x && targetsquare.y == currSquare.y + 1
				|| targetsquare.x == currSquare.x && targetsquare.y == currSquare.y - 1
				|| targetsquare.x == currSquare.x + 1 && targetsquare.y == currSquare.y + 1
				|| targetsquare.x == currSquare.x - 1 && targetsquare.y == currSquare.y + 1
				|| targetsquare.x == currSquare.x + 1 && targetsquare.y == currSquare.y - 1
				|| targetsquare.x == currSquare.x - 1 && targetsquare.y == currSquare.y - 1
				)
			{
				auto piece = GetPieceAtCase(targetsquare);
				if (piece != nullptr)
				{
					if (team == WHITE)
						m_winner = m_winner = m_teamBlack->KillPiece(piece);
					else
						m_winner = m_winner = m_teamWhite->KillPiece(piece);
				}
				m_currentPiece->SetPosition(m_quadrillage[(int)targetsquare.x][(int)targetsquare.y], targetsquare.x, targetsquare.y);
				bRet = true;
			}
			break;
		case PAWN:
			switch (team)
			{
			case WHITE:
				if (targetsquare.y == currSquare.y - 1 && targetsquare.x == currSquare.x)
				{
					if (GetPieceAtCase(targetsquare) == nullptr)
					{
						m_currentPiece->setLap(m_currentPiece->GetLap() + 1);
						m_currentPiece->SetPosition(m_quadrillage[(int)targetsquare.x][(int)targetsquare.y], targetsquare.x, targetsquare.y);
						bRet = true;
					}
					else
						bRet = false;
				}
				else if (targetsquare.y == currSquare.y - 2 && targetsquare.x == currSquare.x && m_currentPiece->GetLap() == 1)
				{
					if (GetPieceAtCase(targetsquare) == nullptr && GetPieceAtCase(sf::Vector2f(targetsquare.x, targetsquare.y + 1)) == nullptr) //TODO : a tester
					{
						m_currentPiece->SetPosition(m_quadrillage[(int)targetsquare.x][(int)targetsquare.y], targetsquare.x, targetsquare.y);
						m_currentPiece->setLap(m_currentPiece->GetLap() + 1);
						bRet = true;
					}
					else
						bRet = false;
				}
				else if ((targetsquare.y == currSquare.y - 1 && targetsquare.x == currSquare.x + 1)
					|| (targetsquare.y == currSquare.y - 1 && targetsquare.x == currSquare.x - 1)) //TODO :peut bouffer en arriere ?
				{
					auto piece = GetPieceAtCase(targetsquare);
					if (piece != nullptr)
					{
						if (piece->GetColor() != team)
						{
							m_winner = m_teamBlack->KillPiece(piece);
							m_currentPiece->SetPosition(m_quadrillage[(int)targetsquare.x][(int)targetsquare.y], targetsquare.x, targetsquare.y);
							bRet = true;
						}
					}
					else
						bRet = false;
				}
				break;
			case BLACK:
				if (targetsquare.y == currSquare.y + 1 && targetsquare.x == currSquare.x)
				{
					if (GetPieceAtCase(targetsquare) == nullptr)
					{
						m_currentPiece->setLap(m_currentPiece->GetLap() + 1);
						m_currentPiece->SetPosition(m_quadrillage[(int)targetsquare.x][(int)targetsquare.y], targetsquare.x, targetsquare.y);
						bRet = true;
					}
					else
						bRet = false;
				}
				else if (targetsquare.y == currSquare.y + 2 && targetsquare.x == currSquare.x && m_currentPiece->GetLap() == 1)
				{
					if (GetPieceAtCase(targetsquare) == nullptr && GetPieceAtCase(sf::Vector2f(targetsquare.x, targetsquare.y - 1)) == nullptr) //TODO : a tester
					{
						m_currentPiece->SetPosition(m_quadrillage[(int)targetsquare.x][(int)targetsquare.y], targetsquare.x, targetsquare.y);
						m_currentPiece->setLap(m_currentPiece->GetLap() + 1);
						bRet = true;
					}
					else
						bRet = false;
				}
				else if ((targetsquare.y == currSquare.y + 1 && targetsquare.x == currSquare.x + 1)//TODO :peut bouffer en arriere ?
					|| (targetsquare.y == currSquare.y + 1 && targetsquare.x == currSquare.x - 1))
				{
					auto piece = GetPieceAtCase(targetsquare);
					if (piece != nullptr)
					{
						if (piece->GetColor() != team)
						{
							m_winner = m_teamWhite->KillPiece(piece);
							m_currentPiece->SetPosition(m_quadrillage[(int)targetsquare.x][(int)targetsquare.y], targetsquare.x, targetsquare.y);
							bRet = true;
						}
					}
					else
						bRet = false;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		return bRet;
	}

	sf::Vector2f GetCase(sf::Vector2i position)
	{
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				if (position.x >= m_quadrillage[i][j].x && position.x < m_quadrillage[i][j].x + (m_boardSize.x / 8)
					&& position.y >= m_quadrillage[i][j].y && position.y < m_quadrillage[i][j].y + (m_boardSize.y / 8))
					return sf::Vector2f(i, j);
			}
		}
	}
	sf::Vector2f GetCase(sf::Vector2f position)
	{
		return GetCase(sf::Vector2i(position.x, position.y));
	}
	bool IsCaseEmpty(sf::Vector2i indices)
	{
		return true;
	}
	Piece* GetPieceAtCase(sf::Vector2f pos)
	{
		for (auto piece : *(m_teamWhite->GetPieces()))
		{
			if (piece->GetCase().x == pos.x && piece->GetCase().y == pos.y)
				return piece;
		}
		for (auto piece : *(m_teamBlack->GetPieces()))
		{
			if (piece->GetCase().x == pos.x && piece->GetCase().y == pos.y)
				return piece;
		}
		return nullptr;
	}

	void PlayMove() { m_moveSound->play(); }
	EColor* GetWinner() { return m_winner; }
};


#endif
