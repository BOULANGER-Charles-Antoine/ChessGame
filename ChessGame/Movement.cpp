#include "Chessboard.h"

// D�tecte tous les mouvements possibles pour la pi�ce s�lectionn� de l'�chiquier pass� en param�tre
void Chessboard::Movement::detectMovement(Chessboard &chessboard) {
	// Si aucune pi�ce n'est s�lectionn�, il n'y aucun mouvement � d�tecter
	if (chessboard.selectedPiece == nullptr)
		return;

	const int lengthChessboard = chessboard.length;
	const auto typeSelectedPiece = str_type(*chessboard.selectedPiece);
	Position newPosition = chessboard.selectedPiece->getPosition();

	// Pi�ce s�lectionn� est un pion
	if (typeSelectedPiece == typeid(Pawn).name()) {
		int direction;
		chessboard.isCurrentPlayerWhite ? direction = UP[0] : direction = DOWN[0];

		// Le pion a d�j� boug� donc il ne peut avancer que d'une case
		if (static_pointer_cast<Pawn>(chessboard.selectedPiece)->isMoved()) {
			newPosition += Position(direction, 0);

			if(newPosition.verifyPosition(0, lengthChessboard) && chessboard.grid[newPosition.getX()][newPosition.getY()].isNoPiece()) {
				chessboard.grid[newPosition.getX()][newPosition.getY()].setOnPath(true);
			}
		} else {
			for (int i = 0; i < 2; i++) {
				newPosition.changePosition(chessboard.originalPositionSelectedPiece.getX() + direction * (i + 1), chessboard.originalPositionSelectedPiece.getY());

				if (newPosition.verifyPosition(0, lengthChessboard) && chessboard.grid[newPosition.getX()][newPosition.getY()].isNoPiece())
					chessboard.grid[newPosition.getX()][newPosition.getY()].setOnPath(true);
				else
					break;
			}
		}

		for (int i = -1; i <= 1; i += 2) {
			// V�rification des diagonales devant le pion pour une possible prise
			newPosition.changePosition(chessboard.originalPositionSelectedPiece.getX() + direction, chessboard.originalPositionSelectedPiece.getY() + i);

			if (chessboard.isKarnaugh && !newPosition.verifyPosition(0, lengthChessboard))
				newPosition.resetY(0, lengthChessboard);

			if (newPosition.verifyPosition(0, lengthChessboard) && !chessboard.grid[newPosition.getX()][newPosition.getY()].isNoPiece() && !chessboard.selectedPiece->sameColor(chessboard.grid[newPosition.getX()][newPosition.getY()])) {
				chessboard.grid[newPosition.getX()][newPosition.getY()].setOnPath(true);
			}


			// V�rification des c�t�s du pion pour une possible prise en passant
			newPosition.changePosition(chessboard.originalPositionSelectedPiece.getX(), chessboard.originalPositionSelectedPiece.getY() + i);

			if (chessboard.isKarnaugh && !newPosition.verifyPosition(0, lengthChessboard))
					newPosition.resetY(0, lengthChessboard);
			
			if (newPosition.verifyPosition(0, lengthChessboard) 
				&& !chessboard.grid[newPosition.getX()][newPosition.getY()].isNoPiece()
				&& chessboard.grid[newPosition.getX() + direction][newPosition.getY()].isNoPiece()
				&& !chessboard.selectedPiece->sameColor(chessboard.grid[newPosition.getX()][newPosition.getY()])
				&& chessboard.nextPlayer->getPieceInBag(newPosition) != nullptr
				&& str_type(*(chessboard.nextPlayer->getPieceInBag(newPosition))) == typeid(Pawn).name()
				&& static_pointer_cast<Pawn>(chessboard.nextPlayer->getPieceInBag(newPosition))->getCanEnPassant()) {
				chessboard.grid[newPosition.getX() + direction][newPosition.getY()].setOnPath(true);
			}
		}
	} else if(typeSelectedPiece == typeid(King).name()) { // Pi�ce s�lectionn� est un roi
		int direction[8][2] = { {UP[0], UP[1]},
								{UP_RIGHT[0], UP_RIGHT[1]},
								{RIGHT[0], RIGHT[1]},
								{DOWN_RIGHT[0], DOWN_RIGHT[1]},
								{DOWN[0], DOWN[1]},
								{DOWN_LEFT[0], DOWN_LEFT[1]},
								{LEFT[0], LEFT[1]},
								{UP_LEFT[0], UP_LEFT[1]}
		};

		for (const auto& iDirection : direction) {
			newPosition.changePosition(chessboard.originalPositionSelectedPiece.getX() + iDirection[0], chessboard.originalPositionSelectedPiece.getY() + iDirection[1]);

			if (chessboard.isKarnaugh && !newPosition.verifyPosition(0, lengthChessboard))
				newPosition.resetY(0, lengthChessboard);

			if (newPosition.verifyPosition(0, lengthChessboard) && !chessboard.selectedPiece->sameColor(chessboard.grid[newPosition.getX()][newPosition.getY()])) {
				chessboard.grid[newPosition.getX()][newPosition.getY()].setOnPath(true);
			}
		}

		// D�tection des roques
		if (!static_pointer_cast<King>(chessboard.selectedPiece)->isCastlingDone()) {
			int numberCase;
			chessboard.isCapablanca ? numberCase = 3 : numberCase = 2;

			for (int i = -1; i <= 1; i += 2) {
				if (emptyRowSideKing(chessboard, i) && isRookSide(chessboard, i)) {
					bool castling = true;

					for (int j = 1; j <= numberCase; j++) {
						newPosition.changePosition(chessboard.originalPositionSelectedPiece.getX(), chessboard.originalPositionSelectedPiece.getY() + i * j);

						if(moveCauseCheck(chessboard, newPosition)) {
							makeMove(chessboard, chessboard.originalPositionSelectedPiece, false);
							castling = false;
							break;
						}
					}
					makeMove(chessboard, chessboard.originalPositionSelectedPiece, false);

					newPosition.changePosition(chessboard.originalPositionSelectedPiece.getX(), chessboard.originalPositionSelectedPiece.getY() + i * numberCase);
					chessboard.grid[chessboard.originalPositionSelectedPiece.getX()][chessboard.originalPositionSelectedPiece.getY() + i * numberCase].setOnPath(castling);
				}
			}
		}
	} else {
		// Mouvement du cavalier
		if (typeSelectedPiece == typeid(Knight).name() || typeSelectedPiece == typeid(Cardinal).name() || typeSelectedPiece == typeid(Marshal).name()) {
			const int direction[8][2] = { {KNIGHT_UP_RIGHT[0], KNIGHT_UP_RIGHT[1]},
								          {KNIGHT_UP_LEFT[0], KNIGHT_UP_LEFT[1]},
								          {KNIGHT_RIGHT_UP[0], KNIGHT_RIGHT_UP[1]},
								          {KNIGHT_RIGHT_DOWN[0], KNIGHT_RIGHT_DOWN[1]},
								          {KNIGHT_DOWN_RIGHT[0], KNIGHT_DOWN_RIGHT[1]},
								          {KNIGHT_DOWN_LEFT[0], KNIGHT_DOWN_LEFT[1]},
                                          {KNIGHT_LEFT_UP[0], KNIGHT_LEFT_UP[1]},
								          {KNIGHT_LEFT_DOWN[0], KNIGHT_LEFT_DOWN[1]}
			};

			for (int i = 0; i < 8; i++) {
				newPosition.changePosition(chessboard.originalPositionSelectedPiece.getX() + direction[i][0], chessboard.originalPositionSelectedPiece.getY() + direction[i][1]);

				if (chessboard.isKarnaugh && !newPosition.verifyPosition(0, lengthChessboard))
					newPosition.resetY(0, lengthChessboard);

				if (newPosition.verifyPosition(0, lengthChessboard) && !chessboard.selectedPiece->sameColor(chessboard.grid[newPosition.getX()][newPosition.getY()])) {
					chessboard.grid[newPosition.getX()][newPosition.getY()].setOnPath(true);
				}
			}

			// Si on est un cavalier, pas d'autres mouvements � d�tecter, on quitte la fonction
			if (typeSelectedPiece == typeid(Knight).name())
				return;
		}

		// Mouvement de la tour
		if (typeSelectedPiece == typeid(Rook).name() || typeSelectedPiece == typeid(Marshal).name() || typeSelectedPiece == typeid(Queen).name()) {
			const int direction[4][2] = { {UP[0], UP[1]},
								          {RIGHT[0], RIGHT[1]},
								          {DOWN[0], DOWN[1]},
								          {LEFT[0], LEFT[1]}
			};

			for (int i = 0; i < 4; i++) {
				newPosition.changePosition(chessboard.originalPositionSelectedPiece.getX() + direction[i][0], chessboard.originalPositionSelectedPiece.getY() + direction[i][1]);
				
				do {
					// Hors de l'�chiquier
					if (chessboard.isKarnaugh && !newPosition.verifyPosition(0, lengthChessboard))
						newPosition.resetY(0, lengthChessboard);

					// Si on tombe sur une pi�ce de m�me couleur, on arr�te l�
					if(newPosition.verifyPosition(0, lengthChessboard) && chessboard.selectedPiece->sameColor(chessboard.grid[newPosition.getX()][newPosition.getY()]))
						break;

					// Sinon on marque la case de l'�chiquier comme position possible, et si c'est une pi�ce, c'est forc�ment une pi�ce adverse et on s'arr�te l� aussi
					if(newPosition.verifyPosition(0, lengthChessboard)) {
						chessboard.grid[newPosition.getX()][newPosition.getY()].setOnPath(true);
						if (!chessboard.grid[newPosition.getX()][newPosition.getY()].isNoPiece())
							break;
					} else {
						break;
					}
					
					newPosition += Position(direction[i][0], direction[i][1]);
				} while ((chessboard.isKarnaugh && newPosition != chessboard.originalPositionSelectedPiece) || newPosition.verifyPosition(0, lengthChessboard));
			}

			// Si la pi�ce n'est pas une reine, on a alors fait tous les mouvements possibles pour la pi�ce actuelle
			if (typeSelectedPiece != typeid(Queen).name())
				return;
		} 
		
		// Mouvement du fou
		if (typeSelectedPiece == typeid(Bishop).name() || typeSelectedPiece == typeid(Cardinal).name() || typeSelectedPiece == typeid(Queen).name()) {
			const int direction[4][2] = { {UP_RIGHT[0], UP_RIGHT[1]},
									      {DOWN_RIGHT[0], DOWN_RIGHT[1]},
									      {DOWN_LEFT[0], DOWN_LEFT[1]},
									      {UP_LEFT[0], UP_LEFT[1]}
			};

			for (int i = 0; i < 4; i++) {
				newPosition.changePosition(chessboard.originalPositionSelectedPiece.getX() + direction[i][0], chessboard.originalPositionSelectedPiece.getY() + direction[i][1]);
				
				do {
					// Hors de l'�chiquier
					if (chessboard.isKarnaugh && !newPosition.verifyPosition(0, lengthChessboard))
						newPosition.resetY(0, lengthChessboard);

					// Si on tombe sur une pi�ce de m�me couleur, on arr�te l�
					if (newPosition.verifyPosition(0, lengthChessboard) && chessboard.selectedPiece->sameColor(chessboard.grid[newPosition.getX()][newPosition.getY()]))
						break;

					// Sinon on marque la case de l'�chiquier comme position possible, et si c'est une pi�ce, c'est forc�ment une pi�ce adverse et on s'arr�te l� aussi
					if(newPosition.verifyPosition(0, lengthChessboard)) {
						chessboard.grid[newPosition.getX()][newPosition.getY()].setOnPath(true);
						if (!chessboard.grid[newPosition.getX()][newPosition.getY()].isNoPiece())
							break;
					} else {
						break;
					}
					
					newPosition += Position(direction[i][0], direction[i][1]);
				} while ((chessboard.isKarnaugh && newPosition != chessboard.originalPositionSelectedPiece) || newPosition.verifyPosition(0, lengthChessboard));
			}
		} 
	}
}


// Renvoie la liste des diff�rents mouvements possibles pour la pi�ce s�lectionn�e
vector<Position> Chessboard::Movement::listMovement(Chessboard &chessboard) {
	vector<Position> listPosition;
	
	for (int i = 0; i < chessboard.length; i++) {
		for (int j = 0; j < chessboard.length; j++) {
			// On regarde si on a une situation d'�checs si on bouge la pi�ce � la position (j, i)
			// i et j invers�s pour trier les mouvements selon la lettre puis le chiffre
			if (chessboard.grid[j][i].getOnPath()) {
				// Si on n'a pas une situation d'�checs alors on ajoute la position � la liste des positions possibles
				if (!moveCauseCheck(chessboard, Position(j, i)))
					listPosition.emplace_back(j, i);
				else
					chessboard.grid[j][i].setOnPath(false);
			}
		}
	}

	// On remet l'�chiquier dans l'�tat d'avant les tests
	makeMove(chessboard, chessboard.originalPositionSelectedPiece, false);

	return listPosition;
}


// Affiche la liste des mouvements disponibles et demande la saisie d'une coordonn�e � l'utilisateur, puis fait le mouvement demand�
void Chessboard::Movement::selectMovement(Chessboard &chessboard, vector<Position> possiblePosition) {
	cout << "Liste des mouvements possibles pour cette piece :\n\t";

	// On r��crit la suite en vert clair sur fond noir
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A);
	for (size_t i = 0; i < possiblePosition.size(); i++) {
		// Trier en fonction de la lettre puis du chiffre
		if (i > 0 && possiblePosition[i].getY() % 10 != possiblePosition[i - 1].getY() % 10)
			cout << "\n\t";
		
		cout << static_cast<char>(possiblePosition[i].getY() + 65) << possiblePosition[i].getX() + 1 << "\t";
	}
	// On r��crit la suite en blanc sur fond noir
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
	cout << "\n\n";

	char x = ' ';
	int y = 0;
	Position positionDesired;
	bool loop;

	cout << "Entrez les coordonnees de la case ou vous voulez deplacer la piece (D4 par exemple) : ";
	do {
		// Saisie s�curis�e des coordonn�es
		while (!(cin >> x) || toupper(x) < 65 || toupper(x) > 65 + chessboard.length - 1 || !(cin >> y) || y < 1 || y > chessboard.length) {
			// si ctrl+Z ou ctrl+D OU �chec de saisie
			if (cin.eof() || cin.fail()) {
				cout << "Erreur, entrez les coordonnees de la case ou vous voulez deplacer la piece (une lettre entre A et " << static_cast<char>(65 + chessboard.length - 1) << " suivie d'un chiffre entre 1 et " << chessboard.length << ") : ";
				cin.clear();
				cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // On vide le buffer
			} else {
				cout << "Erreur, veuillez reessayer : ";
			}
		}

		positionDesired.changePosition(y - 1, (int)(toupper(x) - 65));
		loop = find(possiblePosition.begin(), possiblePosition.end(), positionDesired) == possiblePosition.end();

		if (loop)
			cout << "Erreur, vous ne pouvez selectionner cette case, veuillez reessayer : ";
	} while (loop);
	cout << "\n";

	// On fait le mouvement demand�
	makeMove(chessboard, positionDesired, true);
	chessboard.chessboardActualization();
}


// Applique le mouvement d�sir� en prenant en compte les changements des param�tres des pi�ces
void Chessboard::Movement::makeMove(Chessboard& chessboard, const Position& destination, const bool& realMove) {
	// Traitement mouvement  pion
	if(realMove && str_type(*chessboard.selectedPiece) == typeid(Pawn).name()) {
		// Avance de 2 cases -> peut subir une prise en passant au tour prochain
		if(abs(chessboard.originalPositionSelectedPiece.getX() - destination.getX()) == 2) {
			if(chessboard.isCurrentPlayerWhite)
				static_pointer_cast<Pawn>(chessboard.player1.getPieceInBag(chessboard.originalPositionSelectedPiece))->setCanEnPassant(true);
			else
				static_pointer_cast<Pawn>(chessboard.player2.getPieceInBag(chessboard.originalPositionSelectedPiece))->setCanEnPassant(true);
		} else {
			int direction;
			chessboard.isCurrentPlayerWhite ? direction = DOWN[0] : direction = UP[0];
			const Position positionPawnEnPassant = destination + Position(direction, 0);

			if(positionPawnEnPassant.verifyPosition(0, chessboard.length) && chessboard.nextPlayer->getPieceInBag(positionPawnEnPassant) != nullptr && str_type(*(chessboard.nextPlayer->getPieceInBag(positionPawnEnPassant))) == typeid(Pawn).name() && static_pointer_cast<Pawn>(chessboard.nextPlayer->getPieceInBag(positionPawnEnPassant))->getCanEnPassant()) {
				if(chessboard.isCurrentPlayerWhite)
					chessboard.player2.removePiece(positionPawnEnPassant);
				else
					chessboard.player1.removePiece(positionPawnEnPassant);
			}
		}
	}
	
	
	// Traitement mouvement  roi
	if (realMove && str_type(*chessboard.selectedPiece) == typeid(King).name()) {
		// Avance de 2 cases minimum -> c'est un roque
		if (abs(chessboard.originalPositionSelectedPiece.getY() - destination.getY()) == 2 || abs(chessboard.originalPositionSelectedPiece.getY() - destination.getY()) == 3) {
			Position rookCastlingPosition;

			// Petit roque
			if(chessboard.originalPositionSelectedPiece.getY() < destination.getY()) {
				rookCastlingPosition.changePosition(chessboard.originalPositionSelectedPiece.getX(), chessboard.length - 1);
				if (chessboard.isCurrentPlayerWhite) {
					dynamic_pointer_cast<Rook>(chessboard.player1.getPieceInBag(rookCastlingPosition))->setCastlingDone(true);

					chessboard.player1.modifyPieceBag(chessboard.player1.getPieceInBag(rookCastlingPosition), destination + Position(0, -1));
				} else {
					dynamic_pointer_cast<Rook>(chessboard.player2.getPieceInBag(rookCastlingPosition))->setCastlingDone(true);

					chessboard.player2.modifyPieceBag(chessboard.player2.getPieceInBag(rookCastlingPosition), destination + Position(0, -1));
				}
			} else { // Grand roque
				rookCastlingPosition.changePosition(chessboard.selectedPiece->getPosition().getX(), 0);
				if (chessboard.isCurrentPlayerWhite) {
					dynamic_pointer_cast<Rook>(chessboard.player1.getPieceInBag(rookCastlingPosition))->setCastlingDone(true);

					chessboard.player1.modifyPieceBag(chessboard.player1.getPieceInBag(rookCastlingPosition), destination + Position(0, 1));
				} else {
					dynamic_pointer_cast<Rook>(chessboard.player2.getPieceInBag(rookCastlingPosition))->setCastlingDone(true);

					chessboard.player2.modifyPieceBag(chessboard.player2.getPieceInBag(rookCastlingPosition), destination + Position(0, 1));
				}
			}
		}
		
		if (chessboard.isCurrentPlayerWhite)
			static_pointer_cast<King>(chessboard.player1.getPieceInBag(chessboard.originalPositionSelectedPiece))->setCastlingDone(true);
		else
			static_pointer_cast<King>(chessboard.player2.getPieceInBag(chessboard.originalPositionSelectedPiece))->setCastlingDone(true);
	}
	
	// Si une tour bouge, elle ne peut plus faire de roque
	if (realMove && str_type(*chessboard.selectedPiece) == typeid(Rook).name()) {
		if (chessboard.isCurrentPlayerWhite)
			dynamic_pointer_cast<Rook>(chessboard.player1.getPieceInBag(chessboard.originalPositionSelectedPiece))->setCastlingDone(true);
		else
			dynamic_pointer_cast<Rook>(chessboard.player2.getPieceInBag(chessboard.originalPositionSelectedPiece))->setCastlingDone(true);
	}
	
	// Supprimer pi�ce adverse
	if (realMove && !chessboard.grid[destination.getX()][destination.getY()].isNoPiece()) {
		if (chessboard.isCurrentPlayerWhite)
			chessboard.player2.removePiece(destination);
		else
			chessboard.player1.removePiece(destination);
	}
	
	// Appliquer d�placement
	if (chessboard.isCurrentPlayerWhite) {
		chessboard.player1.modifyPieceBag(chessboard.selectedPiece, destination);

		if(realMove)
			for (auto it = chessboard.player2.pieceBag.begin(); it != chessboard.player2.pieceBag.end(); ++it) {
				if (str_type(**it) == typeid(Pawn).name()) {
					static_pointer_cast<Pawn>(*it)->setCanEnPassant(false);
				}
			}
	} else {
		chessboard.player2.modifyPieceBag(chessboard.selectedPiece, destination);

		if(realMove)
			for (auto it = chessboard.player1.pieceBag.begin(); it != chessboard.player1.pieceBag.end(); ++it) {
				if (str_type(**it) == typeid(Pawn).name()) {
					static_pointer_cast<Pawn>(*it)->setCanEnPassant(false);
				}
			}
	}
}


// Renvoie true si il n'y a pas de pi�ce entre le roi et une tour, tour dans la direction pass� en param�tre
bool Chessboard::Movement::emptyRowSideKing(const Chessboard& chessboard, const int& direction) {
	auto position = Position(chessboard.originalPositionSelectedPiece.getX(), chessboard.originalPositionSelectedPiece.getY() + direction);
	
	while (position.getY() > 0 && position.getY() < chessboard.length - 1) {
		if(!chessboard.grid[position.getX()][position.getY()].isNoPiece())
			return false;
		
		position += Position(0, direction);
	}

	// Si la pi�ce � l�extr�mit� du c�t� du roi est une tour, on renvoie vrai
	return chessboard.currentPlayer->getPieceInBag(position) != nullptr && str_type(*chessboard.currentPlayer->getPieceInBag(position)) == typeid(Rook).name();
}


// Renvoie true si une tour peut pratiquer un roque, tour qui est celle dans la direction pass� en param�tre
bool Chessboard::Movement::isRookSide(const Chessboard& chessboard, const int& direction) {
	if (direction != -1 && direction != 1)
		throw invalid_argument("Error 03 : Direction erronee pour le roque.");

	Position position;
	(direction == 1) ? position += Position(0, chessboard.length - 1) : position += Position(0, 0);
	(chessboard.isCurrentPlayerWhite) ? position += Position(0, 0) : position += Position(chessboard.length - 1, 0);
	
	if (!chessboard.grid[position.getX()][position.getY()].isNoPiece() && chessboard.selectedPiece->sameColor(chessboard.grid[position.getX()][position.getY()]) && str_type(*(chessboard.currentPlayer->getPieceInBag(position))) == typeid(Rook).name() && !dynamic_pointer_cast<Rook>(chessboard.currentPlayer->getPieceInBag(position))->isCastlingDone())
		return true;

	return false;
}


// Renvoie true si le joueur actuelle est en situation d'�checs
bool Chessboard::Movement::isCheck(const Chessboard& chessboard) {
	for (auto it = chessboard.nextPlayer->pieceBag.begin(); it != chessboard.nextPlayer->pieceBag.end(); ++it)
		if (pieceCauseCheck(chessboard, *it, false))
			return true;

	return false;
}


// Renvoie true quand le mouvement d'une pi�ce adverse cause un �chec
bool Chessboard::Movement::moveCauseCheck(Chessboard chessboard, const Position& position) {
	// Simulation du mouvement
	const bool presencePieceBeforeMove = !chessboard.grid[position.getX()][position.getY()].isNoPiece();
	makeMove(chessboard, position, false);
	chessboard.chessboardActualization();

	for (auto it = chessboard.nextPlayer->pieceBag.begin(); it != chessboard.nextPlayer->pieceBag.end(); ++it) {
		// On ignore la pi�ce qui vient d'�tre pris
		if ((*it)->getPosition() == position)
			continue;
		
		// On v�rifie si la pi�ce cause un �chec
		if (pieceCauseCheck(chessboard, *it, presencePieceBeforeMove))
			return true;
	}
		
	return false;
}


// Renvoie true si une pi�ce adverse provoque une situation d'�checs
bool Chessboard::Movement::pieceCauseCheck(const Chessboard& chessboard, const shared_ptr<Piece>& piece, const bool& presencePieceBeforeMove) {
	// Stockage de la position du roi dans le sac de pi�ce du joueur courant
	Position kingPosition;
	for (auto it = chessboard.currentPlayer->pieceBag.begin(); it != chessboard.currentPlayer->pieceBag.end(); ++it)
		if (str_type(**it) == typeid(King).name()) {
			kingPosition = (*it)->getPosition();
			break;
		}

	const int lengthChessboard = chessboard.length;
	Position newPosition;

	if (str_type(*piece) == typeid(Pawn).name()) {
		// Direction du pion adverse
		int direction;
		chessboard.isCurrentPlayerWhite ? direction = DOWN[0] : direction = UP[0];

		for (int i = -1; i <= 1; i += 2) {
			newPosition.changePosition(piece->getPosition().getX() + direction, piece->getPosition().getY() + i);

			if (chessboard.isKarnaugh && !newPosition.verifyPosition(0, lengthChessboard))
				newPosition.resetY(0, lengthChessboard);
			
			if (newPosition.verifyPosition(0, lengthChessboard) && (!piece->sameColor(chessboard.grid[newPosition.getX()][newPosition.getY()]) || presencePieceBeforeMove) && newPosition == kingPosition) {
				return true;
			}
		}
	} else if (str_type(*piece) == typeid(King).name()) {
		const int direction[8][2] = { {UP[0], UP[1]},
								      {UP_RIGHT[0], UP_RIGHT[1]},
								      {RIGHT[0], RIGHT[1]},
								      {DOWN_RIGHT[0], DOWN_RIGHT[1]},
								      {DOWN[0], DOWN[1]},
								      {DOWN_LEFT[0], DOWN_LEFT[1]},
								      {LEFT[0], LEFT[1]},
								      {UP_LEFT[0], UP_LEFT[1]}
		};

		for (int i = 0; i < 8; i++) {
			newPosition.changePosition(piece->getPosition().getX() + direction[i][0], piece->getPosition().getY() + direction[i][1]);

			if (chessboard.isKarnaugh && !newPosition.verifyPosition(0, lengthChessboard))
				newPosition.resetY(0, lengthChessboard);

			if (newPosition.verifyPosition(0, lengthChessboard) && (!piece->sameColor(chessboard.grid[newPosition.getX()][newPosition.getY()]) || presencePieceBeforeMove) && newPosition == kingPosition) {
				return true;
			}
		}
	} else {
		// Mouvement du cavalier
		if (str_type(*piece) == typeid(Knight).name() || str_type(*piece) == typeid(Cardinal).name() || str_type(*piece) == typeid(Marshal).name()) {
			int direction[8][2] = { {KNIGHT_UP_RIGHT[0], KNIGHT_UP_RIGHT[1]},
									{KNIGHT_UP_LEFT[0], KNIGHT_UP_LEFT[1]},
									{KNIGHT_RIGHT_UP[0], KNIGHT_RIGHT_UP[1]},
									{KNIGHT_RIGHT_DOWN[0], KNIGHT_RIGHT_DOWN[1]},
									{KNIGHT_DOWN_RIGHT[0], KNIGHT_DOWN_RIGHT[1]},
									{KNIGHT_DOWN_LEFT[0], KNIGHT_DOWN_LEFT[1]},
									{KNIGHT_LEFT_UP[0], KNIGHT_LEFT_UP[1]},
									{KNIGHT_LEFT_DOWN[0], KNIGHT_LEFT_DOWN[1]}
			};

			for (int i = 0; i < 8; i++) {
				newPosition.changePosition(piece->getPosition().getX() + direction[i][0], piece->getPosition().getY() + direction[i][1]);

				if (chessboard.isKarnaugh && !newPosition.verifyPosition(0, lengthChessboard))
					newPosition.resetY(0, lengthChessboard);

				if (newPosition.verifyPosition(0, lengthChessboard) && (!piece->sameColor(chessboard.grid[newPosition.getX()][newPosition.getY()]) || presencePieceBeforeMove) && newPosition == kingPosition) {
					return true;
				}
			}
		}

		// Mouvement de la tour
		if (str_type(*piece) == typeid(Rook).name() || str_type(*piece) == typeid(Marshal).name() || str_type(*piece) == typeid(Queen).name()) {
			const int direction[4][2] = { {UP[0], UP[1]},
									      {RIGHT[0], RIGHT[1]},
									      {DOWN[0], DOWN[1]},
									      {LEFT[0], LEFT[1]}
			};
			
			for (int i = 0; i < 4; i++) {
				const auto vecteur = Position(direction[i][0], direction[i][1]);
				newPosition.changePosition(piece->getPosition().getX() + direction[i][0], piece->getPosition().getY() + direction[i][1]);
				
				do {
					// Hors de l'�chiquier
					if (chessboard.isKarnaugh && !newPosition.verifyPosition(0, lengthChessboard))
						newPosition.resetY(0, lengthChessboard);
					
					if (newPosition.verifyPosition(0, lengthChessboard) && piece->sameColor(chessboard.grid[newPosition.getX()][newPosition.getY()]) && !presencePieceBeforeMove)
						break;
					
					if (newPosition.verifyPosition(0, lengthChessboard)) {
						if(newPosition == kingPosition)
							return true;

						if (!chessboard.grid[newPosition.getX()][newPosition.getY()].isNoPiece() && !piece->sameColor(chessboard.grid[newPosition.getX()][newPosition.getY()]))
							break;
					} else {
						break;
					}

					if (chessboard.isKarnaugh && newPosition == piece->getPosition())
						break;
					newPosition += vecteur;
				} while (chessboard.isKarnaugh || newPosition.verifyPosition(0, lengthChessboard));
			}
		}

		// Mouvement du fou
		if (str_type(*piece) == typeid(Bishop).name() || str_type(*piece) == typeid(Cardinal).name() || str_type(*piece) == typeid(Queen).name()) {
			const int direction[4][2] = { {UP_RIGHT[0], UP_RIGHT[1]},
									      {DOWN_RIGHT[0], DOWN_RIGHT[1]},
									      {DOWN_LEFT[0], DOWN_LEFT[1]},
									      {UP_LEFT[0], UP_LEFT[1]}
			};

			for (int i = 0; i < 4; i++) {
				const auto vecteur = Position(direction[i][0], direction[i][1]);
				newPosition.changePosition(piece->getPosition().getX() + direction[i][0], piece->getPosition().getY() + direction[i][1]);
				
				do {
					// Hors de l'�chiquier
					if (chessboard.isKarnaugh && !newPosition.verifyPosition(0, lengthChessboard))
						newPosition.resetY(0, lengthChessboard);

					// Si on tombe sur une pi�ce de m�me couleur, on arr�te l�
					if (newPosition.verifyPosition(0, lengthChessboard) && piece->sameColor(chessboard.grid[newPosition.getX()][newPosition.getY()]) && !presencePieceBeforeMove)
						break;

					// Sinon on marque la case de l'�chiquier comme position possible, et si c'est une pi�ce, c'est forc�ment une pi�ce adverse et on s'arr�te l� aussi
					if (newPosition.verifyPosition(0, lengthChessboard)) {
						if (newPosition == kingPosition)
							return true;

						if (!chessboard.grid[newPosition.getX()][newPosition.getY()].isNoPiece())
							break;
					} else {
						break;
					}
					
					if (chessboard.isKarnaugh && newPosition == piece->getPosition())
						break;
					newPosition += vecteur;
				} while (chessboard.isKarnaugh || newPosition.verifyPosition(0, lengthChessboard));
			}
		}
	}

	return false;
}


// Envoie les informations concernant les echecs et mat ou les pat
void Chessboard::Movement::isCheckMateOrIsPat(Chessboard chessboard, bool& isCheckMate, bool& isPat) {
	isCheckMate = true;

	for (auto it = chessboard.currentPlayer->pieceBag.begin(); it != chessboard.currentPlayer->pieceBag.end(); ++it) {
		chessboard.setSelectedPiece(*it);
		detectMovement(chessboard);

		if (!listMovement(chessboard).empty()) {
			chessboard.setSelectedPiece(nullptr);
			isCheckMate = false;
			break;
		}
			
		chessboard.setSelectedPiece(nullptr);
	}

	isPat = isCheckMate && !isCheck(chessboard);
}


// Envoie les informations concernant les nulles
void Chessboard::Movement::isNul(Chessboard chessboard, bool& isNul) {
	isNul = false;

	if(chessboard.currentPlayer->pieceBag.size() == 1) {
		if (chessboard.nextPlayer->pieceBag.size() == 1) {
			isNul = true;
		} else if(chessboard.nextPlayer->pieceBag.size() == 2) {
			for (auto it = chessboard.nextPlayer->pieceBag.begin(); it < chessboard.nextPlayer->pieceBag.end(); ++it)
				if (str_type(**it) == typeid(Knight).name() || str_type(**it) == typeid(Bishop).name())
					isNul = true;
			
			if(chessboard.isKarnaugh && chessboard.isCapablanca) {
				for (auto it = chessboard.nextPlayer->pieceBag.begin(); it < chessboard.nextPlayer->pieceBag.end(); ++it)
					if (str_type(**it) == typeid(Marshal).name() || str_type(**it) == typeid(Cardinal).name())
						isNul = true;
			}
			
		} else if(chessboard.isKarnaugh && chessboard.nextPlayer->pieceBag.size() == 3) {
			int countKnight = 0;
			int countBishop = 0;
			for (auto it = chessboard.nextPlayer->pieceBag.begin(); it < chessboard.nextPlayer->pieceBag.end(); ++it)
				if (str_type(**it) == typeid(Knight).name()) {
					countKnight++;
				} else if(str_type(**it) == typeid(Bishop).name()) {
					countBishop++;
				}

			isNul = countKnight == 2 || countBishop == 2;
		}
	}
}
