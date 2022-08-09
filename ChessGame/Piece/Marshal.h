#ifndef MARSHAL_H
#define MARSHAL_H

#include "Rook.h"
#include "Knight.h"

class Marshal : virtual public Piece, public Knight, public Rook  {
	constexpr static int VALUE = 15;

public:
	Marshal(const Color& colorMarshal, const int& x, const int& y) : Piece(Name::MARSHAL, colorMarshal, VALUE, x, y, 'M'), Knight(colorMarshal, x, y), Rook(colorMarshal, x, y)
	{}
};

#endif