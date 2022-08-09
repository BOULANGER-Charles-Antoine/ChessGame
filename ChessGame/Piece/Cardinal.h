#ifndef CARDINAL_H
#define CARDINAL_H

#include "Knight.h"
#include "Bishop.h"

class Cardinal : public Knight, public Bishop {
	constexpr static int VALUE = 9;

public:
	Cardinal(const Color& colorCardinal, const int& x, const int& y) : Piece(Name::CARDINAL, colorCardinal, VALUE, x, y, 'C'), Knight(colorCardinal, x, y), Bishop(colorCardinal, x, y)
	{}
};

#endif