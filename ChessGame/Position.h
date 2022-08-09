#ifndef POSITION_H
#define POSITION_H

#include <ostream>

using std::ostream;

class Position {
private:
	int x;
	int y;

public:
	// Constructeurs : - Constructeur par défaut
	//                 - Constructeur d'initialisation
	//Position() : x(0), y(0) {}
	Position(const int& x = 0, const int& y = 0) : x(x), y(y) {}

	// Getter
	int getX() const { return x; }
	int getY() const { return y; }

	// Méthodes
	void changePosition(const int &newX, const int &newY);
	bool verifyPosition(const int& min, const int& max) const;
	void resetY(const int& min, const int& max);

	// Surcharge des opérateurs <<, ==, !=, + et +=
	friend ostream& operator <<(ostream& out, const Position& position);
	friend bool operator ==(const Position& position1, const Position& position2);
	friend bool operator !=(const Position& position1, const Position& position2);
	friend Position operator +(const Position& position1, const Position& position2);
	friend Position& operator +=(Position& position1, const Position& position2);
};

#endif