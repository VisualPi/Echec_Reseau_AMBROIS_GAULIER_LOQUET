#pragma once

#include "Utils.h"
#include "Piece.hpp"
#include "Team.hpp"
#include "ChessBoard.hpp"

class ChessBoard {
private:
	Team*			m_teamWhite;
	Team*			m_teamBlack;
	Piece*			m_currentPiece;

	EColor*			m_winner;

public:
	ChessBoard() {
	}

	void InitTeam() {
		m_teamWhite = new Team(WHITE);
		m_teamBlack = new Team(BLACK);

		m_teamWhite->Init();
		m_teamBlack->Init();
		m_winner = nullptr;
	}

	std::vector<Vector2i>* GetReachablePositionsForSelectedPiece(Vector2i currentPosition, EColor team) {
		std::vector<Vector2i>* reachablePositionsForSelectedPiece = new std::vector<Vector2i>();

		for(int y = 0; y < 8; ++y) {
			for(int x = 0; x < 8; ++x) {
				Vector2i temp(x, y);
				//Modifier AskForMovement pour pas deplacer
				if(AskForMovement(currentPosition, temp, team, false)) {
					reachablePositionsForSelectedPiece->push_back(temp);
				}
			}
		}

		return reachablePositionsForSelectedPiece;
	}

	Piece* GetPieceAtCase(Vector2i pos) {
		for(auto piece : *(m_teamWhite->GetPieces())) {
			if(piece->GetCase().x == pos.x && piece->GetCase().y == pos.y)
				return piece;
		}
		for(auto piece : *(m_teamBlack->GetPieces())) {
			if(piece->GetCase().x == pos.x && piece->GetCase().y == pos.y)
				return piece;
		}
		return nullptr;
	}

	void PlayMove() {
		// m_moveSound->play(); 
		// Envoyer le fait de jouer un son au client
	}

	EColor* GetWinner() {
		return m_winner;
	}

	void sendPlaySound() {

	}

	bool AskForMovement(Vector2i currentPosition, Vector2i targetPosition, EColor team, bool move) {
		Vector2i targetsquare = targetPosition;
		Vector2i currSquare = currentPosition;
		bool bRet = false;

		switch(m_currentPiece->GetType()) {
		case ROOK:
			if(targetsquare.y != currSquare.y && targetsquare.x == currSquare.x) //VERTICAL
			{
				auto piece = GetPieceAtCase(targetsquare);
				if((piece != nullptr && piece->GetColor() != team) || piece == nullptr) //s'il y a quelqu'un de la team adverse ou personne
				{
					bRet = true; // setter a defaut a true ici
					if(targetsquare.y > currSquare.y) {
						for(int i = currSquare.y + 1; i < targetsquare.y; ++i) {
							if(GetPieceAtCase(Vector2i(currSquare.x, i)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else {
						for(int i = currSquare.y - 1; i > targetsquare.y; --i) {
							if(GetPieceAtCase(Vector2i(currSquare.x, i)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					if(bRet && move) {
						m_currentPiece->SetPosition(targetsquare.x, targetsquare.y);
						if(piece != nullptr) {
							sendPlaySound();
							if(team == WHITE)
								m_winner = m_teamBlack->KillPiece(piece);
							else
								m_winner = m_teamWhite->KillPiece(piece);
						}

					}
				}
				else
					bRet = false;
			}
			else if(targetsquare.y == currSquare.y && targetsquare.x != currSquare.x)//HORIZONTAL
			{
				auto piece = GetPieceAtCase(targetsquare);
				if((piece != nullptr && piece->GetColor() != team) || piece == nullptr) //s'il y a quelqu'un de la team adverse ou personne
				{
					bRet = true; // setter a defaut a true ici
					if(targetsquare.x > currSquare.x) {
						for(int i = currSquare.x + 1; i < targetsquare.x; ++i) {
							if(GetPieceAtCase(Vector2i(i, currSquare.y)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else {
						for(int i = currSquare.x - 1; i > targetsquare.x; --i) {
							if(GetPieceAtCase(Vector2i(i, currSquare.y)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					if(bRet && move) {
						m_currentPiece->SetPosition(targetsquare.x, targetsquare.y);
						if(piece != nullptr) {
							sendPlaySound();
							if(team == WHITE)
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
			if(targetsquare.x == currSquare.x - 1 && targetsquare.y == currSquare.y - 2
			   || targetsquare.x == currSquare.x + 1 && targetsquare.y == currSquare.y - 2
			   || targetsquare.x == currSquare.x - 1 && targetsquare.y == currSquare.y + 2
			   || targetsquare.x == currSquare.x + 1 && targetsquare.y == currSquare.y + 2
			   || targetsquare.x == currSquare.x + 2 && targetsquare.y == currSquare.y - 1
			   || targetsquare.x == currSquare.x + 2 && targetsquare.y == currSquare.y + 1
			   || targetsquare.x == currSquare.x - 2 && targetsquare.y == currSquare.y - 1
			   || targetsquare.x == currSquare.x - 2 && targetsquare.y == currSquare.y + 1
			   ) {
				auto piece = GetPieceAtCase(targetsquare);
				if(move) {
					m_currentPiece->SetPosition(targetsquare.x, targetsquare.y);
				}
				if(piece != nullptr && move) {
					sendPlaySound();
					if(team == WHITE)
						m_winner = m_teamBlack->KillPiece(piece);
					else
						m_winner = m_teamWhite->KillPiece(piece);
				}
				bRet = true;
			}
			break;
		case BISHOP:
			if((targetsquare.y != currSquare.y && targetsquare.x != currSquare.x)
			   && (abs(targetsquare.y - currSquare.y) == abs(targetsquare.x - currSquare.x))) //VERTICAL
			{
				auto piece = GetPieceAtCase(targetsquare);
				if((piece != nullptr && piece->GetColor() != team) || piece == nullptr) //s'il y a quelqu'un de la team adverse ou personne
				{
					bRet = true; // setter a defaut a true ici
					if(targetsquare.y > currSquare.y && targetsquare.x > currSquare.x) {
						for(int i = currSquare.x + 1, j = currSquare.y + 1; i < targetsquare.x, j < targetsquare.y; ++i, ++j) {
							if(GetPieceAtCase(Vector2i(i, j)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else if(targetsquare.y < currSquare.y && targetsquare.x < currSquare.x) {
						for(int i = currSquare.x - 1, j = currSquare.y - 1; i > targetsquare.x, j > targetsquare.y; --i, --j) {
							if(GetPieceAtCase(Vector2i(i, j)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else if(targetsquare.y < currSquare.y && targetsquare.x > currSquare.x) {
						for(int i = currSquare.x + 1, j = currSquare.y - 1; i < targetsquare.x, j > targetsquare.y; ++i, --j) {
							if(GetPieceAtCase(Vector2i(i, j)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else if(targetsquare.y > currSquare.y && targetsquare.x < currSquare.x) {
						for(int i = currSquare.x - 1, j = currSquare.y + 1; i > targetsquare.x, j < targetsquare.y; --i, ++j) {
							if(GetPieceAtCase(Vector2i(i, j)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					if(bRet && move) {
						m_currentPiece->SetPosition(targetsquare.x, targetsquare.y);
						if(piece != nullptr) {
							sendPlaySound();
							if(team == WHITE)
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
			if(targetsquare.y != currSquare.y && targetsquare.x == currSquare.x) //VERTICAL
			{
				auto piece = GetPieceAtCase(targetsquare);
				if((piece != nullptr && piece->GetColor() != team) || piece == nullptr) //s'il y a quelqu'un de la team adverse ou personne
				{
					bRet = true; // setter a defaut a true ici
					if(targetsquare.y > currSquare.y) {
						for(int i = currSquare.y + 1; i < targetsquare.y; ++i) {
							if(GetPieceAtCase(Vector2i(currSquare.x, i)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else {
						for(int i = currSquare.y - 1; i > targetsquare.y; --i) {
							if(GetPieceAtCase(Vector2i(currSquare.x, i)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					if(bRet && move) {
						m_currentPiece->SetPosition(targetsquare.x, targetsquare.y);
						if(piece != nullptr) {
							sendPlaySound();
							if(team == WHITE)
								m_winner = m_teamBlack->KillPiece(piece);
							else
								m_winner = m_teamWhite->KillPiece(piece);
						}

					}
				}
				else
					bRet = false;
			}
			else if(targetsquare.y == currSquare.y && targetsquare.x != currSquare.x)//HORIZONTAL
			{
				auto piece = GetPieceAtCase(targetsquare);
				if((piece != nullptr && piece->GetColor() != team) || piece == nullptr) //s'il y a quelqu'un de la team adverse ou personne
				{
					bRet = true; // setter a defaut a true ici
					if(targetsquare.x > currSquare.x) {
						for(int i = currSquare.x + 1; i < targetsquare.x; ++i) {
							if(GetPieceAtCase(Vector2i(i, currSquare.y)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else {
						for(int i = currSquare.x - 1; i > targetsquare.x; --i) {
							if(GetPieceAtCase(Vector2i(i, currSquare.y)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					if(bRet && move) {
						m_currentPiece->SetPosition(targetsquare.x, targetsquare.y);
						if(piece != nullptr) {
							sendPlaySound();
							if(team == WHITE)
								m_winner = m_teamBlack->KillPiece(piece);
							else
								m_winner = m_teamWhite->KillPiece(piece);
						}

					}
				}
				else
					bRet = false;
			}
			else if((targetsquare.y != currSquare.y && targetsquare.x != currSquare.x)
					&& (abs(targetsquare.y - currSquare.y) == abs(targetsquare.x - currSquare.x))) //VERTICAL
			{
				auto piece = GetPieceAtCase(targetsquare);
				if((piece != nullptr && piece->GetColor() != team) || piece == nullptr) //s'il y a quelqu'un de la team adverse ou personne
				{
					bRet = true; // setter a defaut a true ici
					if(targetsquare.y > currSquare.y && targetsquare.x > currSquare.x) {
						for(int i = currSquare.x + 1, j = currSquare.y + 1; i < targetsquare.x, j < targetsquare.y; ++i, ++j) {
							if(GetPieceAtCase(Vector2i(i, j)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else if(targetsquare.y < currSquare.y && targetsquare.x < currSquare.x) {
						for(int i = currSquare.x - 1, j = currSquare.y - 1; i > targetsquare.x, j > targetsquare.y; --i, --j) {
							if(GetPieceAtCase(Vector2i(i, j)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else if(targetsquare.y < currSquare.y && targetsquare.x > currSquare.x) {
						for(int i = currSquare.x + 1, j = currSquare.y - 1; i < targetsquare.x, j > targetsquare.y; ++i, --j) {
							if(GetPieceAtCase(Vector2i(i, j)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					else if(targetsquare.y > currSquare.y && targetsquare.x < currSquare.x) {
						for(int i = currSquare.x - 1, j = currSquare.y + 1; i > targetsquare.x, j < targetsquare.y; --i, ++j) {
							if(GetPieceAtCase(Vector2i(i, j)) != nullptr) {
								bRet = false; //quelqu'un est sur le chemin
								break;
							}
						}
					}
					if(bRet && move) {
						m_currentPiece->SetPosition(targetsquare.x, targetsquare.y);
						if(piece != nullptr) {
							sendPlaySound();
							if(team == WHITE)
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
			if(targetsquare.x == currSquare.x + 1 && targetsquare.y == currSquare.y
			   || targetsquare.x == currSquare.x - 1 && targetsquare.y == currSquare.y
			   || targetsquare.x == currSquare.x && targetsquare.y == currSquare.y + 1
			   || targetsquare.x == currSquare.x && targetsquare.y == currSquare.y - 1
			   || targetsquare.x == currSquare.x + 1 && targetsquare.y == currSquare.y + 1
			   || targetsquare.x == currSquare.x - 1 && targetsquare.y == currSquare.y + 1
			   || targetsquare.x == currSquare.x + 1 && targetsquare.y == currSquare.y - 1
			   || targetsquare.x == currSquare.x - 1 && targetsquare.y == currSquare.y - 1
			   ) {
				auto piece = GetPieceAtCase(targetsquare);
				if(move) {
					m_currentPiece->SetPosition(targetsquare.x, targetsquare.y);
				}
				if(piece != nullptr && move) {
					sendPlaySound();
					if(team == WHITE)
						m_winner = m_winner = m_teamBlack->KillPiece(piece);
					else
						m_winner = m_winner = m_teamWhite->KillPiece(piece);
				}
				bRet = true;
			}
			break;
		case PAWN:
			switch(team) {
			case WHITE:
				if(targetsquare.y == currSquare.y - 1 && targetsquare.x == currSquare.x) {
					if(GetPieceAtCase(targetsquare) == nullptr) {
						if(move) {
							m_currentPiece->SetPosition(targetsquare.x, targetsquare.y);
							m_currentPiece->setLap(m_currentPiece->GetLap() + 1);
						}
						bRet = true;
					}
					else
						bRet = false;
				}
				else if(targetsquare.y == currSquare.y - 2 && targetsquare.x == currSquare.x && m_currentPiece->GetLap() == 1) {
					if(GetPieceAtCase(targetsquare) == nullptr && GetPieceAtCase(Vector2i(targetsquare.x, targetsquare.y + 1)) == nullptr) //TODO : a tester
					{
						if(move) {
							m_currentPiece->SetPosition(targetsquare.x, targetsquare.y);
							m_currentPiece->setLap(m_currentPiece->GetLap() + 1);
						}
						bRet = true;
					}
					else
						bRet = false;
				}
				else if((targetsquare.y == currSquare.y - 1 && targetsquare.x == currSquare.x + 1)
						|| (targetsquare.y == currSquare.y - 1 && targetsquare.x == currSquare.x - 1)) //TODO :peut bouffer en arriere ?
				{
					auto piece = GetPieceAtCase(targetsquare);
					if(piece != nullptr) {
						if(piece->GetColor() != team) {
							if(move) {
								sendPlaySound();
								m_winner = m_teamBlack->KillPiece(piece);
								m_currentPiece->SetPosition(targetsquare.x, targetsquare.y);
							}
							bRet = true;
						}
					}
					else
						bRet = false;
				}
				break;
			case BLACK:
				if(targetsquare.y == currSquare.y + 1 && targetsquare.x == currSquare.x) {
					if(GetPieceAtCase(targetsquare) == nullptr) {
						if(move) {
							m_currentPiece->setLap(m_currentPiece->GetLap() + 1);
							m_currentPiece->SetPosition(targetsquare.x, targetsquare.y);
						}
						bRet = true;
					}
					else
						bRet = false;
				}
				else if(targetsquare.y == currSquare.y + 2 && targetsquare.x == currSquare.x && m_currentPiece->GetLap() == 1) {
					if(GetPieceAtCase(targetsquare) == nullptr && GetPieceAtCase(Vector2i(targetsquare.x, targetsquare.y - 1)) == nullptr) //TODO : a tester
					{
						if(move) {
							m_currentPiece->SetPosition(targetsquare.x, targetsquare.y);
							m_currentPiece->setLap(m_currentPiece->GetLap() + 1);
						}
						bRet = true;
					}
					else
						bRet = false;
				}
				else if((targetsquare.y == currSquare.y + 1 && targetsquare.x == currSquare.x + 1)//TODO :peut bouffer en arriere ?
						|| (targetsquare.y == currSquare.y + 1 && targetsquare.x == currSquare.x - 1)) {
					auto piece = GetPieceAtCase(targetsquare);
					if(piece != nullptr) {
						if(piece->GetColor() != team) {
							if(move) {
								sendPlaySound();
								m_winner = m_teamWhite->KillPiece(piece);
								m_currentPiece->SetPosition(targetsquare.x, targetsquare.y);
							}
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
};