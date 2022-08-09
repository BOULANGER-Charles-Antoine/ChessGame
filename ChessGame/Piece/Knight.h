#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : virtual public Piece {
	constexpr static int VALUE = 3;

public:
	Knight(const Color& colorKnight, const int& x, const int& y) : Piece(Name::KNIGHT, colorKnight, VALUE, x, y, 'N')
	{}
};

#endif