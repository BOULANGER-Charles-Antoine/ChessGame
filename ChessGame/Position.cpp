#include "Position.h"

// Modifie les coordonn�es x et y de la position
void Position::changePosition(const int &newX, const int &newY) {
	x = newX;
	y = newY;
}



bool Position::verifyPosition(const int& min, const int& max) const {
	return x >= min && x < max && y >= min && y < max;

	/*if (x < min || x >= max || y < min || y >= max)
		return false;

	return true;*/
}


// Modifie la coordonn�e y de la position, utilis� avec l'option Karnaugh
void Position::resetY(const int& min, const int& max) {
	if (y > max - 1)
		changePosition(x, y - max);
	else if (y < min)
		changePosition(x, max + y);
}


// Affiche une position sur la sortie 
ostream& operator<<(ostream& out, const Position& position) {
	return out << "(" << position.x << ", " << position.y << ")";
}


// Renvoie true si deux positions ont les m�mes coordonn�es
bool operator==(const Position& position1, const Position& position2) {
	return position1.x == position2.x && position1.y == position2.y;
}


// Renvoie true si deux positions ont les m�mes coordonn�es
bool operator!=(const Position& position1, const Position& position2) {
	return !(position1 == position2);
}


// Additionne les coordonn�es de deux positions
Position operator+(const Position& position1, const Position& position2) {
	return {position1.x + position2.x, position1.y + position2.y};
}


// Additionne les coordonn�es de deux points et les sauvegarde dans la position de gauche
Position& operator+=(Position& position1, const Position& position2) {
	return position1 = position1 + position2;
}