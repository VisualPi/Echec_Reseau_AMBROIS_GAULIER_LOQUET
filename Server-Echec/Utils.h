#pragma once

//typedef struct Vector2f {
//	float x;
//	float y;
//
//	Vector2f(float newX, float newY)
//		: x(newX), y(newY) {}
//} Vector2f;

typedef struct Vector2i {
	int x;
	int y;

	Vector2i(int newX, int newY)
		: x(newX), y(newY) {}
} Vector2i;

enum EColor { WHITE, BLACK };
enum ETypePiece { ROOK, KNIGHT, BISHOP, QUEEN, KING, PAWN };