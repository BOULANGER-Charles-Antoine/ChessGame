#ifndef PIECE_H
#define PIECE_H

#include "../Enum.h"
#include "../Position.h"
#include <Windows.h>

using Enum::Color;
using Enum::Name;

class Piece {
protected:
	Name name;
	Color color;
	int value;
	Position position;
	char charName;
	bool isSelected;
	bool onPath;

public:
	// Constructeurs : - Constructeur par défaut
	//                 - Constructeur d'initialisation
	Piece() : name(Name::NONE), color(Color::NONE), value(0), charName(' '), isSelected(false), onPath(false) {}
	Piece(const Name& name, const Color& color, const int& value, const int& x, const int& y, const char& charName) : name(name),
	                                                                                                                  color(color),
		                                                                                                              value(value),
	                                                                                                                  position(x, y),
	                                                                                                                  charName(charName),
	                                                                                                                  isSelected(false),
	                                                                                                                  onPath(false)
	{}

	// Destructeur
	virtual ~Piece() = default;

	// Getter
	Position getPosition() const { return position; }
	bool getOnPath() const { return onPath; }
	Color getColor() const { return color; }
	int getValue() const { return value; }

	// Setter
	void setIsSelected(const bool& newIsSelected) { isSelected = newIsSelected; }
	void setPosition(const Position& newPosition) { position = newPosition; }
	void setOnPath(const bool& newOnPath) { onPath = newOnPath; }

	// Méthodes
	bool isNoPiece() const { return name == Name::NONE; }
	bool sameColor(const Piece& piece) const { return color == piece.color; }

	// Surcharge des opérateurs << et ==
	friend ostream& operator<<(ostream& out, const Piece& piece);
	friend bool operator==(const Piece& lhs, const Piece& rhs);
};

#endif