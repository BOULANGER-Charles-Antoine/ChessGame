#include "Player.h"

// Initialise le sac de pièce du joueur
void Player::pieceBagInitialization(const bool& isCapablanca) {
	int posX;
	int posXPawn;
	int numberRow;

	if (color == Color::WHITE) {
		posX = 0;
		posXPawn = 1;
	} else {
		isCapablanca ? (posX = 9, posXPawn = 8) : (posX = 7, posXPawn = 6);
	}

	isCapablanca ? numberRow = 10 : numberRow = 8;

	// Crée les pièces du joueur aux bonne coordonnées
	if (isCapablanca) {
		pieceBag.push_back(make_shared<Rook>(Rook(color, posX, 0)));
		pieceBag.push_back(make_shared<Knight>(Knight(color, posX, 1)));
		pieceBag.push_back(make_shared<Cardinal>(Cardinal(color, posX, 2)));
		pieceBag.push_back(make_shared<Bishop>(Bishop(color, posX, 3)));
		pieceBag.push_back(make_shared<Queen>(Queen(color, posX, 4)));
		pieceBag.push_back(make_shared<King>(King(color, posX, 5)));
		pieceBag.push_back(make_shared<Bishop>(Bishop(color, posX, 6)));
		pieceBag.push_back(make_shared<Marshal>(Marshal(color, posX, 7)));
		pieceBag.push_back(make_shared<Knight>(Knight(color, posX, 8)));
		pieceBag.push_back(make_shared<Rook>(Rook(color, posX, 9)));
	} else {
		pieceBag.push_back(make_shared<Rook>(Rook(color, posX, 0)));
		pieceBag.push_back(make_shared<Knight>(Knight(color, posX, 1)));
		pieceBag.push_back(make_shared<Bishop>(Bishop(color, posX, 2)));
		pieceBag.push_back(make_shared<Queen>(Queen(color, posX, 3)));
		pieceBag.push_back(make_shared<King>(King(color, posX, 4)));
		pieceBag.push_back(make_shared<Bishop>(Bishop(color, posX, 5)));
		pieceBag.push_back(make_shared<Knight>(Knight(color, posX, 6)));
		pieceBag.push_back(make_shared<Rook>(Rook(color, posX, 7)));
	}

	for (int i = 0; i < numberRow; i++)
		pieceBag.push_back(make_shared<Pawn>(Pawn(color, posXPawn, i)));
}


// Modifie la position de la pièce dans le sac du joueur
void Player::modifyPieceBag(const shared_ptr<Piece>& pieceToModify, const Position& newPosition) const {
	bool changeHasMovedPawn;
	bool changeHasMovedKing;

	// Opération ternaire pour savoir si la pièce est un pion ou un roi, i.e. pièce avec restriction de déplacement
	str_type(*pieceToModify) == typeid(Pawn).name() ? changeHasMovedPawn = true : changeHasMovedPawn = false;
	str_type(*pieceToModify) == typeid(King).name() ? changeHasMovedKing = true : changeHasMovedKing = false;

	for (auto it = pieceBag.begin(); it != pieceBag.end(); ++it) {
		// Recherche de la pièce à modifier
		if (pieceToModify == *it) {
			// Modification de la position
			(*it)->setPosition(newPosition);

			// On met à jour les variables indiquant que le roi ou le pion a bougé
			if (changeHasMovedPawn)
				static_pointer_cast<Pawn>(*it)->setMoved(true);

			if (changeHasMovedKing)
				static_pointer_cast<King>(*it)->setCastlingDone(true);

			return;
		}
	}
}


// Trie les pièces par leur valeur
bool trieByPieceValue(shared_ptr<Piece> piece1, shared_ptr<Piece> piece2) {
	return piece1->getValue() > piece2->getValue();
}


// Suppression de la pièce ayant la position passé en paramètre
void Player::removePiece(const Position& destination) {
	for (auto it = pieceBag.begin(); it != pieceBag.end(); ++it) {
		// On cherche la pièce à supprimer du sac
		if (destination == (*it)->getPosition()) {
			// On l'ajoute au sac de pièce mangé
			(*it)->setIsSelected(false);
			eatenPieceBag.push_back(move(*it));
			std::sort(eatenPieceBag.begin(), eatenPieceBag.end(), trieByPieceValue);

			// On l'efface du sac de pièce du joueur avant de quitter la fonction
			pieceBag.erase(it);
			return;
		}
	}
}


// Ajoute la pièce passé en paramètre dans le sac du joueur
void Player::addPieceToBag(const shared_ptr<Piece>& newPiece) {
	pieceBag.push_back(newPiece);
}


// Affiche les paramètres du joueur
ostream& operator<<(ostream& out, const Player& player) {
	out << "Nom du joueur : " << player.name << "\n";
	out << "Type du joueur : " << (player.isHuman ? "humain" : "ordinateur") << "\n";
	out << "Joue les " << (player.color == Color::WHITE ? "blancs" : "noirs") << "\n";
	out << "Son sac de piece est compose de : ";
	for (auto it = player.pieceBag.begin(); it != player.pieceBag.end(); ++it) {
		out << **it << " " << str_type(**it) << "\t";
	}

	return out << "\n";
}