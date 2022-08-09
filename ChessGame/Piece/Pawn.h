#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn : public Piece {
	constexpr static int VALUE = 1;

private:
	bool hasMoved;
	bool canEnPassant;

public:
	Pawn(const Color& colorPawn, const int& x, const int& y) : Piece(Name::PAWN, colorPawn, VALUE, x, y, 'P'), hasMoved(false), canEnPassant(false)
	{}

	// Getter
	bool isMoved() const { return hasMoved; }
	bool getCanEnPassant() const { return canEnPassant; }

	// Setter
	void setMoved(bool newHasMoved) { hasMoved = newHasMoved; }
	void setCanEnPassant(bool newCanEnPassant) { canEnPassant = newCanEnPassant; }
};

#endif