#ifndef KING_H
#define KING_H

#include "Piece.h"

class King: public Piece {
	constexpr static int VALUE = 200;

private:
	// true si le roi a bougé ou si un rock a été fait
	boolean castlingDone;

public:
	King(const Color& colorKing, const int& x, const int& y) : Piece(Name::KING, colorKing, VALUE, x, y, 'K'), castlingDone(false)
	{}

	// Getter
	boolean isCastlingDone() const { return castlingDone; }

	// Setter
	void setCastlingDone(const boolean& newCastlingDone) { castlingDone = newCastlingDone; }
};

#endif