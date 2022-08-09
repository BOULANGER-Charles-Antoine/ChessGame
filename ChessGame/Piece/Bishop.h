#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

class Bishop : virtual public Piece {
	constexpr static int VALUE = 3;

public:
	Bishop(const Color& colorBishop, const int& x, const int& y) : Piece(Name::BISHOP, colorBishop, VALUE, x, y, 'B')
	{}
};

#endif