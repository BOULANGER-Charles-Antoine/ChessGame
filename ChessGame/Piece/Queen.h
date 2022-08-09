#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

class Queen : public Piece {
	constexpr static int VALUE = 25;

public:
	Queen(const Color& colorQueen, const int& x, const int& y) : Piece(Name::QUEEN, colorQueen, VALUE, x, y, 'Q')
	{}
};

#endif