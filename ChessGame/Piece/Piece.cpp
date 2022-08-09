#include "Piece.h"

// Name          | Value
// ---------------------
// Black         | 0
// Blue          | 1
// Green         | 2
// Cyan          | 3
// Red           | 4
// Magenta       | 5
// Brown         | 6
// Light Gray    | 7
// Dark Gray     | 8
// Light Blue    | 9
// Light Green   | A
// Light Cyan    | B
// Light Red     | C
// Light Magenta | D
// Yellow        | E
// White         | F


// Affiche une pièce
ostream& operator<<(ostream& out, const Piece& piece) {
	// Fond et texte en noir
	unsigned short colorText = 0x00;

	// Mise en blanc du texte
	if (piece.color == Color::BLACK)
		colorText = 0x0F;
	
	if (piece.color == Color::NONE && piece.onPath) { // Mise en bleu de la piece car est nul et est sur le chemin
		colorText = 0x11;
	} else if (piece.isSelected) { // Mise en vert du texte
		colorText += 0x20;
	} else if (piece.onPath) { // Mise en rouge du texte, car est non-nul et est sur le chemin 
		colorText += 0x40;
	} else {
		// Sinon couleur normale à appliquer
		if (piece.color == Color::WHITE) {
			colorText = 0xF0; // Texte blanc sur fond noir
		} else if (piece.color == Color::BLACK) {
			colorText = 0x0F; // Texte noir sur fond blanc
		}
	}

	// On écrit selon les paramètres précédents
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorText);

	out << " " << piece.charName << " ";

	// On réécrit la suite en blanc sur fond noir
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
	return out;
}


// Renvoie true si deux pièces sont identiques, i.e. elles sont de la même couleur, ont le même nom et la même position
bool operator==(const Piece& lhs, const Piece& rhs) {
	return lhs.color == rhs.color && lhs.name == rhs.name && lhs.position == rhs.position;
}
