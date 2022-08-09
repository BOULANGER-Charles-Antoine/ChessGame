#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : virtual public Piece {
	constexpr static int VALUE = 5;

private:
	boolean castlingDone;

public:
	Rook(const Color& colorRook, const int& x, const int& y) : Piece(Name::ROOK, colorRook, VALUE, x, y, 'R'), castlingDone(false)
	{}

	// Getter
	boolean isCastlingDone() const { return castlingDone; }

	// Setter
	void setCastlingDone(const boolean& newCastlingDone) { castlingDone = newCastlingDone; }
};

#endif