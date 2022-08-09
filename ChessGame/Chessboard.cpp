#include "Chessboard.h"

// Actualisation de l'échiquier
void Chessboard::chessboardActualization() {
	if(isCurrentPlayerWhite) {
		currentPlayer = make_shared<Player>(player1);
		nextPlayer = make_shared<Player>(player2);
	} else {
		currentPlayer = make_shared<Player>(player2);
		nextPlayer = make_shared<Player>(player1);
	}

	// Remplissage de la grille par des non-pièces
	for (auto it = grid.begin(); it != grid.end(); ++it)
		for (auto jt = it->begin(); jt != it->end(); ++jt)
			*jt = emptyPiece;

	// Remplissage de la grille par les pièces du premier joueur
	for (auto it = player1.pieceBag.begin(); it != player1.pieceBag.end(); ++it)
		grid[(*it)->getPosition().getX()][(*it)->getPosition().getY()] = **it;

	// Remplissage de la grille par les pièces du deuxième joueur
	for (auto it = player2.pieceBag.begin(); it != player2.pieceBag.end(); ++it)
		grid[(*it)->getPosition().getX()][(*it)->getPosition().getY()] = **it;
}


// Demande au joueur de sélectionner une pièce et l'affiche en vert une fois la saisie confirmée
void Chessboard::selectPiece() {
	char x = ' ';
	int y = 0;
	Position positionPieceDesired;
	bool loop;
	
	cout << "C'est au tour de " << currentPlayer->name << " (les " << (isCurrentPlayerWhite ? "blancs" : "noirs") << ") de jouer\n\n";
	setSelectedPiece(nullptr);
	chessboardActualization();
	cout << *this;

	cout << "\nEntrez les coordonnees de la piece que vous voulez jouer (D2 par exemple) : ";
	// Demande des coordonnées de la pièce désirée
	do {
		// Saisie sécurisée des coordonnées
		while (!(cin >> x) || toupper(x) < 65 || toupper(x) > 65 + length - 1 || !(cin >> y) || y < 1 || y > length) {
			// si ctrl+Z ou ctrl+D OU échec de saisie
			if (cin.eof() || cin.fail()) {
				cout << "Erreur, entrez les coordonnees de la piece que vous voulez jouer (une lettre entre A et " << static_cast<char>(65 + length - 1) << " suivie d'un chiffre entre 1 et " << length << ") : ";
				cin.clear();
				cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // On vide le buffer
			} else {
				cout << "Erreur, veuillez reessayer : ";
			}
		}

		positionPieceDesired.changePosition((y - 1), (int)(toupper(x) - 65));
		loop = !isASelectionnablePiece(positionPieceDesired);

		if (loop)
			cout << "Erreur, vous ne pouvez selectionner cette piece, veuillez reessayer : ";
	} while (loop);
	
	chessboardActualization();
}


// Vérification de la pièce sélectionné, vérifie si elle est possédé par le joueur voulant la jouer et si il a entré les bonnes coordonnées
bool Chessboard::isASelectionnablePiece(const Position &positionPieceDesired) {
	// Si la pièce se trouve au position précisé et est dans le sac du joueur courant, alors on renvoie true
	for (auto it = currentPlayer->pieceBag.begin(); it != currentPlayer->pieceBag.end(); ++it) {
		if ((*it)->getPosition() == positionPieceDesired) {
			(*it)->setIsSelected(true);
			setSelectedPiece(*it);

			return true;
		}
	}

	return false;
}


// Fait la promotion des pions
void Chessboard::promotion() {
	int limit;
	isCurrentPlayerWhite ? limit = length - 1 : limit = 0;

	if (str_type(*selectedPiece) == typeid(Pawn).name() && selectedPiece->getPosition().getX() == limit) {
		char typePiece = '\0';

		// Saisie sécurisée de la ligne
		cout << "Votre pion a ete promu, entrez la lettre du type de piece que vous voulez \n(Q : Reine, N : Cavalier, B: Fou, R : Tour";
		if (isCapablanca)
			cout << ", C: Cardinal, M : Marechal";
		cout << ") : ";

		while (!(cin >> typePiece) || (toupper(typePiece) != 'Q' && toupper(typePiece) != 'N' && toupper(typePiece) != 'B' && toupper(typePiece) != 'R' && (!isCapablanca || (toupper(typePiece) != 'C' && toupper(typePiece) != 'M')))) {
			// si ctrl+Z ou ctrl+D OU échec de saisie
			if (cin.eof() || cin.fail()) {
				cout << "Erreur, entrez une lettre correspondant ala piece que vous desirez : ";
				cin.clear();
				cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // On vide le buffer
			} else {
				cout << "Erreur, la lettre entree ne correspond a aucune piece, veuillez recommencer : ";
			}
		}

		shared_ptr<Piece> newPiece;
		const Position positionSelectedPiece = selectedPiece->getPosition();

		switch (toupper(typePiece)) {
			case 'Q':
				newPiece = make_shared<Queen>(Queen(currentPlayer->getColor(), positionSelectedPiece.getX(), positionSelectedPiece.getY()));
				break;
			case 'N':
				newPiece = make_shared<Knight>(Knight(currentPlayer->getColor(), positionSelectedPiece.getX(), positionSelectedPiece.getY()));
				break;
			case 'B':
				newPiece = make_shared<Bishop>(Bishop(currentPlayer->getColor(), positionSelectedPiece.getX(), positionSelectedPiece.getY()));
				break;
			case 'R':
				newPiece = make_shared<Rook>(Rook(currentPlayer->getColor(), positionSelectedPiece.getX(), positionSelectedPiece.getY()));
				break;
			case 'C':
				newPiece = make_shared<Cardinal>(Cardinal(currentPlayer->getColor(), positionSelectedPiece.getX(), positionSelectedPiece.getY()));
				break;
			case 'M':
				newPiece = make_shared<Marshal>(Marshal(currentPlayer->getColor(), positionSelectedPiece.getX(), positionSelectedPiece.getY()));
				break;
		default:
				throw invalid_argument("Error 01 : Saisie erronee lors de la promotion.");
		}

		if (isCurrentPlayerWhite) {
			player1.removePiece(positionSelectedPiece);
			player1.addPieceToBag(newPiece);
		} else {
			player2.removePiece(positionSelectedPiece);
			player2.addPieceToBag(newPiece);
		}

		chessboardActualization();
	}
}


// Produit un tour de jeu
void Chessboard::round() {
	vector<Position> listPosition;

	// Tant qu'une pièce sélectionné ne peut pas bouger, on redemande une nouvelle saisie
	do {
		selectPiece();
		Movement::detectMovement(*this);
		listPosition = Movement::listMovement(*this);
		
		if (listPosition.empty()) {
			system("cls");
			cout << *this;
			setSelectedPiece(nullptr);
			cout << "Cette piece ne peut pas bouger, veuillez en choisir une autre \n";
			
			pause();
			system("cls");
			if (Movement::isCheck(*this)) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4F);
				cout << "Vous etes en situation d'echecs.";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
				cout << "\n\n";
			}
		}
	} while (listPosition.empty());

	// On indique en rouge si le joueur actuelle est en situation d'échec
	system("cls");
	if (Movement::isCheck(*this)) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4F);
		cout << "Vous etes en situation d'echecs.";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
		cout << "\n\n";
	}

	// On affiche les mouvements possibles et on demande à en choisir un
	cout << *this << "\n";
	Movement::selectMovement(*this, listPosition);

	// On vérifie si une promotion a lieu puis on réinitialise la pièce sélectionné et on passe au joueur suivant
	promotion();
	setSelectedPiece(nullptr);
	switchPlayer();
	system("cls");
}


// Produit le déroulement entier d'une partie d'échecs
void Chessboard::game() {
	bool isCheckMate = false;
	bool isPat = false;
	bool isNul = false;

	while (!isCheckMate && !isPat && !isNul) {
		if (Movement::isCheck(*this)) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4F);
			cout << "Vous etes en situation d'echecs.";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
			cout << "\n\n";
		}
		round();
		
		Movement::isCheckMateOrIsPat(*this, isCheckMate, isPat);
		Movement::isNul(*this, isNul);
	}

	win(isCheckMate, isPat, isNul);
}


// On affiche l'écran de fin du jeu
void Chessboard::win(bool isCheckMate, bool isPat, bool isNul) const {
	if (!isCheckMate && !isPat && !isNul)
		throw invalid_argument("Erreur 02 : Argument de fin de partie erronee.");

	cout << *this << "\n";

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF0);
	if (isPat)
		cout << "PAT, aucun gagnant, vous vous etes bien battu !!";
	else if (isCheckMate)
		cout << nextPlayer->name << " (les " << (!isCurrentPlayerWhite ? "blancs" : "noirs") << ") a gagne, belle partie !!";
	else if(isNul)
		cout << "Partie nul par manque de materiel, vous vous etes bien battu !!";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
	cout << "\n\n";

	pause();
	system("cls");
}


// Affichage de l'échiquier
std::ostream& operator<<(std::ostream& out, const Chessboard& chessboard) {
	for (int i = chessboard.length - 1; i >= 0; i--) {
		// Première ligne
		if (i == chessboard.length - 1) {
			out << "     "; // 4 caractère vides "_____"

			// Lettre de la colonne au début
			for (int j = 0; j < chessboard.length; j++)
				out << static_cast<char>(65 + j) << "   "; // 3 caractères vides "_|_"

			out << "\n";
		}

		// Chiffre de la colonne au début
		out << (i + 1);
		if (i + 1 < 10)
			out << " ";
		out << " |";

		for (int j = 0; j < chessboard.length; j++)
			out << chessboard.grid[i][j] << "|";

		out << " " << (i + 1) << "          "; // Chiffre de la colonne à la fin

		int heightEatenPieceBag;
		const int nbPieceOnLine = 8;
		chessboard.isCapablanca ? heightEatenPieceBag = 4 : heightEatenPieceBag = 3;
		const int spaceBetweenEatenPieceBag = 2;

		// Affichage des pièces mangés par chacun des joueurs
		if(i < chessboard.length - 1 - heightEatenPieceBag - spaceBetweenEatenPieceBag) {
			cout << "    |";
			size_t jBegin = nbPieceOnLine * (chessboard.length - 1 - heightEatenPieceBag - spaceBetweenEatenPieceBag - i - 1);
			size_t jEnd = nbPieceOnLine * (chessboard.length - 1 - heightEatenPieceBag - spaceBetweenEatenPieceBag - i);
			for (size_t j = jBegin; j < jEnd; j++) {
				if(j < chessboard.player1.eatenPieceBag.size())
					cout << *chessboard.player1.eatenPieceBag[j] << "| ";
				else
					break;
			}
		} else if(i == chessboard.length - 1 - heightEatenPieceBag - spaceBetweenEatenPieceBag) { // Affichage du nom du joueur
			cout << chessboard.player1.getName() << " :";
		} else if(i < chessboard.length - 1 && i > chessboard.length - 1 - heightEatenPieceBag) {
			cout << "    |";
			size_t jBegin = nbPieceOnLine * (chessboard.length - 1 - i - 1);
			size_t jEnd = nbPieceOnLine * (chessboard.length - 1 - i);
			for (size_t j = jBegin; j < jEnd; j++) {
				if (j < chessboard.player2.eatenPieceBag.size())
					cout << *chessboard.player2.eatenPieceBag[j] << "| ";
				else
					break;
			}
		} else if (i == chessboard.length - 1) { // Affichage du nom du joueur
			cout << chessboard.player2.getName() << " :";
		}

		cout << "\n";
	}

	out << "     "; // 6 caractère vides "______"
	// Lettre de la colonne à la fin
	for (int i = 0; i < chessboard.length; i++) {
		out << static_cast<char>(65 + i) << "   "; // 3 caractères vides "_|_"
	}

	// Retour chariot pour être sous le plateau
	out << "\n";
	return out;
}