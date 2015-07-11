#pragma once

#include "SFML\Graphics.hpp"
#include "SFML\Window.hpp"
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

enum EColor { WHITE, BLACK };
enum ETypePiece { ROOK, KNIGHT, BISHOP, QUEEN, KING, PAWN };

#include "Piece.hpp"
#include "Team.hpp"
#include "TextureLoader.hpp"
#include "ChessBoard.hpp"