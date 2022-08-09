#ifndef PLAYER_H
#define PLAYER_H

#include "Enum.h"
#include "Piece/allPiece.h"
#include <string>
#include <vector>
#include <memory>
#include <ostream>
#include <iostream>
#include <algorithm>

using std::cout;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::static_pointer_cast;
using std::vector;
using std::ostream;
using std::move;
using std::iterator;

class Chessboard;

// Fonction permettant de renvoyer le type d'un objet passé en paramètre (utilisé pour les pièces)
template <typename T> char const* str_type(const T& obj) {
	return typeid(obj).name();
}

class Player {
	friend class Chessboard;
	friend ostream& operator<<(ostream& out, const Chessboard& chessboard);

private:
	string name;
	bool isHuman;
	vector<shared_ptr<Piece>> pieceBag;
	vector<shared_ptr<Piece>> eatenPieceBag;
	Color color;

public:
	// Constructeurs
	Player() : name(" "), isHuman(true), pieceBag(vector<shared_ptr<Piece>>()), color(Color::NONE) {}
	Player(const string& name, const bool& isHuman, const bool& isCapablanca, const Color& color) : name(name), isHuman(isHuman), color(color) {
		pieceBagInitialization(isCapablanca);
		eatenPieceBag = vector<shared_ptr<Piece>>();
	}

	// Getters
	shared_ptr<Piece> getPieceInBag(const Position& positionPiece) const {
		for (auto it = pieceBag.begin(); it != pieceBag.end(); ++it)
			if ((*it)->getPosition() == positionPiece)
				return *it;

		return nullptr;
	}
	Color getColor() const { return color; }
	string getName() const { return name; }

	// Méthodes
	void pieceBagInitialization(const bool& isCapablanca);
	void modifyPieceBag(const shared_ptr<Piece>& pieceToModify, const Position& newPosition) const;
	void removePiece(const Position& destination);
	void addPieceToBag(const shared_ptr<Piece>& newPiece);

	// Surcharge d'opérateur
	friend ostream& operator<<(ostream& out, const Player& player);
};

#endif