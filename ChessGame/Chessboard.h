#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "Enum.h"
#include "Player.h"
#include "Piece/Piece.h"

using std::cout;
using std::cin;
using std::vector;
using std::shared_ptr;
using std::move;
using std::dynamic_pointer_cast;
using std::invalid_argument;


static void pause() {
	cout << "Appuyez sur entree pour continuer ...\n";
	cin.ignore();
	cin.get();
}


class Chessboard {
	// D�finition de la non pi�ce
	const Piece emptyPiece = Piece();
private:
	vector<vector<Piece>> grid;
	int length;
	bool isCapablanca;
	bool isKarnaugh;
	Player player1;
	Player player2;
	bool isCurrentPlayerWhite;
	shared_ptr<Piece> selectedPiece;
	Position originalPositionSelectedPiece;
	bool checkMate;
	shared_ptr<Player> currentPlayer;
	shared_ptr<Player> nextPlayer;

public:
	// Constructeur
	Chessboard(bool isCapablanca, bool isKarnaugh, string namePlayer1, string namePlayer2, bool isPlayer2Human) : isCapablanca(isCapablanca),
		                                                                                                          isKarnaugh(isKarnaugh),
																										          player1(move(namePlayer1), true, isCapablanca, Color::WHITE),
		                                                                                                          player2(move(namePlayer2), isPlayer2Human, isCapablanca, Color::BLACK),
		                                                                                                          isCurrentPlayerWhite(true),
	                                                                                                              selectedPiece(new Piece()),
	                                                                                                              originalPositionSelectedPiece(0, 0),
	                                                                                                              checkMate(false),
	                                                                                                              currentPlayer(make_shared<Player>(player1)),
	                                                                                                              nextPlayer(make_shared<Player>(player2)) {
		// D�finition de la grille
		isCapablanca ? length = 10 : length = 8;
		grid.resize(length);
		for (int i = 0; i < length; i++)
			grid[i].resize(length);

		chessboardActualization();
	}

	// Setter
	void setSelectedPiece(const shared_ptr<Piece>& newSelectedPiece) {
		if(newSelectedPiece != nullptr) {
			selectedPiece = newSelectedPiece;
			originalPositionSelectedPiece = selectedPiece->getPosition();
		} else {
			// On r�initialise les param�tres de la pi�ce s�lectionn�e
			for (auto it = currentPlayer->pieceBag.begin(); it != currentPlayer->pieceBag.end(); ++it) {
				if (selectedPiece != nullptr && (*it)->getPosition() == selectedPiece->getPosition()) {
					(*it)->setIsSelected(false);
					break;
				}
			}

			// On r�initialise les param�tres des pi�ces de l'�chiquier
			for (auto it = grid.begin(); it != grid.end(); ++it) {
				for (auto jt = it->begin(); jt != it->end(); ++jt) {
					jt->setIsSelected(false);
					jt->setOnPath(false);
				}
			}

			// On r�initialise le pointeur de la pi�ce s�lectionn�e et sa position
			selectedPiece = newSelectedPiece;
			originalPositionSelectedPiece = Position();
		}
	}
	void switchPlayer() {
		isCurrentPlayerWhite = !isCurrentPlayerWhite;

		// Switch des shared_ptr
		shared_ptr<Player> temp = move(currentPlayer);
		currentPlayer = move(nextPlayer);
		nextPlayer = move(temp);
	}

	// M�thodes
	void chessboardActualization();
	void selectPiece();
	bool isASelectionnablePiece(const Position& positionPieceDesired);
	void promotion();
	void round();
	void game();
	void win(bool isCheckMate, bool isPat, bool isNul) const;

	// Surcharge d'op�rateur
	friend ostream& operator<<(ostream& out, const Chessboard& chessboard);

	// Classe g�rant les mouvements des pi�ces et les situations d'�checs, de pat et d'�checs et mat
	class Movement {
	private:
		// Constantes de d�placement pour les 8 mouvements de base
		static constexpr int UP[2] = { 1, 0 };
		static constexpr int UP_RIGHT[2] = { 1, 1 };
		static constexpr int RIGHT[2] = { 0, 1 };
		static constexpr int DOWN_RIGHT[2] = { -1, 1 };
		static constexpr int DOWN[2] = { -1, 0 };
		static constexpr int DOWN_LEFT[2] = { -1, -1 };
		static constexpr int LEFT[2] = { 0, -1 };
		static constexpr int UP_LEFT[2] = { 1, -1 };

		// Constantes de d�placement pour les 8 d�placements du cavalier
		static constexpr int KNIGHT_UP_RIGHT[2] = { 2, 1 };
		static constexpr int KNIGHT_UP_LEFT[2] = { 2, -1 };
		static constexpr int KNIGHT_RIGHT_UP[2] = { 1, 2 };
		static constexpr int KNIGHT_RIGHT_DOWN[2] = { -1, 2 };
		static constexpr int KNIGHT_DOWN_RIGHT[2] = { -2, 1 };
		static constexpr int KNIGHT_DOWN_LEFT[2] = { -2, -1 };
		static constexpr int KNIGHT_LEFT_UP[2] = { 1, -2 };
		static constexpr int KNIGHT_LEFT_DOWN[2] = { -1, -2 };
	public:
		// M�thodes de gestion des mouvement
		static void detectMovement(Chessboard& chessboard);
		static vector<Position> listMovement(Chessboard& chessboard);
		static void selectMovement(Chessboard& chessboard, vector<Position> possiblePosition);
		static void makeMove(Chessboard& chessboard, const Position& destination, const bool& realMove);

		// M�thodes de gestion des mouvements sp�ciaux et d'�checs
		static bool emptyRowSideKing(const Chessboard& chessboard, const int& direction);
		static bool isRookSide(const Chessboard& chessboard, const int& direction);
		static bool isCheck(const Chessboard& chessboard);
		static bool moveCauseCheck(Chessboard chessboard, const Position& position);
		static bool pieceCauseCheck(const Chessboard& chessboard, const shared_ptr<Piece>& piece, const bool& presencePieceBeforeMove);
		static void isCheckMateOrIsPat(Chessboard chessboard, bool& isCheckMate, bool& isPat);
		static void isNul(Chessboard chessboard, bool& isNul);
	};
};

#endif