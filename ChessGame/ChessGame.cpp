#include <iomanip>
#include <iostream>
#include "Windows.h"

#include "Player.h"
#include "Chessboard.h"

using std::exception;

static void mainMenu();
static void configurationGameMenu(bool onePlayer);
static void aide();

void mon_gestionnaire() {
    cout << "Exception non gérée reçue !\n";

    exit(-1);
}



// Menu principal
static void mainMenu() {
    int choice = '\0';
    do {
        cout << "=========================================================================\n";
        cout << "================================CHESSGAME================================\n";
        cout << "=========================================================================\n\n";

        cout << "------------------------------------------\n";
        cout << " 1. Commencer une nouvelle partie 1 joueur\n";
        cout << " 2. Commencer une nouvelle partie 2 joueurs\n";
        cout << " 3. Aide\n";
        cout << " 4. Quitter\n";
        cout << "------------------------------------------\n\n";

        cout << "Entrez votre choix : ";
        while (!(cin >> choice) || choice < 1 || choice > 4) {
            // si ctrl+Z ou ctrl+D OU échec de saisie
            if (cin.eof() || cin.fail()) {
                cout << "Erreur, entrez un chiffre entre 1 et 4 : ";
                cin.clear();
                cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // On vide le buffer
            } else {
                cout << "Erreur, veuillez reessayer : ";
            }
        }

        switch (choice) {
            case 1:
                cout << "Mode de jeu pas encore disponible.\n";
                pause();

                break;
            case 2:
	            try {
                    configurationGameMenu(false);
	            } catch (exception &e) {
                    system("cls");
                    cout << e.what() << "\n";
                    pause();
	            }

                break;
            case 3:
                aide();
                pause();

                break;
            case 4:
                cout << "\nA bientot !!\n\n";
                pause();

                break;
            default:
                break;
        }
        system("cls");
    } while (choice != 4);
}


// Demande à l'utilisateur avec quelle option de jeu il veut jouer
void configurationGameMenu(bool onePlayer) {
    system("cls");

	string namePlayer1;
    string namePlayer2;
    char choice;

    // Prénom du premier joueur
    if(onePlayer)
		cout << "Entrez votre nom : ";
    else
        cout << "Entrez le nom du joueur qui va jouer les blancs : ";
    while (!(cin >> namePlayer1)) {
        // si ctrl+Z ou ctrl+D OU échec de saisie
        if (cin.eof() || cin.fail()) {
            cout << "Erreur, veuillez reessayer : ";
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // On vide le buffer
        } 
    }

    // Prénom du deuxième joueur
    if(onePlayer) {
        namePlayer2 = "Bot";
    } else {
        cout << "Entrez le nom du joueur qui va jouer les noirs : ";
        while (!(cin >> namePlayer2)) {
            // si ctrl+Z ou ctrl+D OU échec de saisie
            if (cin.eof() || cin.fail()) {
                cout << "Erreur, veuillez reessayer : ";
                cin.clear();
                cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // On vide le buffer
            }
        }
    }

    // Demande variante Capablanca
    cout << "Voulez-vous jouer avec la variante de Capablanca ? ";
    while (!(cin >> choice)) {
        // si ctrl+Z ou ctrl+D OU échec de saisie
        if (cin.eof() || cin.fail()) {
            cout << "Erreur, veuillez reessayer : ";
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // On vide le buffer
        }
    }
    const bool isCapablanca = toupper(choice) == 'O';

    // Demande variante de Karnaugh
    cout << "Voulez-vous jouer sur un cylindre (variante de Karnaugh) ? ";
    while (!(cin >> choice)) {
        // si ctrl+Z ou ctrl+D OU échec de saisie
        if (cin.eof() || cin.fail()) {
            cout << "Erreur, veuillez reessayer : ";
            cin.clear();
            cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // On vide le buffer
        }
    }
    const bool isKarnaugh = toupper(choice) == 'O';

    // Génération de l'échiquier tant que l'utilisateur veut rejouer
	do {
        Chessboard chessboard(isCapablanca, isKarnaugh, namePlayer1, namePlayer2, !onePlayer);
        try {
            system("cls");
            chessboard.game();
        } catch (exception &e) {
            cout << e.what() << "\n\n";
        }

        cout << "Voulez-vous rejouer avec les memes parametres de jeu ? ";
        while (!(cin >> choice)) {
            // si ctrl+Z ou ctrl+D OU échec de saisie
            if (cin.eof() || cin.fail()) {
                cout << "Erreur, veuillez reessayer : ";
                cin.clear();
                cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n'); // On vide le buffer
            }
        }
	} while (toupper(choice) == 'O');
}


// Aide du jeu
static void aide() {
    system("cls");


    // Présentation du fonctionnement des pièces
    cout << "I. Fonctionnement piece\n\n";
    cout << " Les pieces classiques sont representes de la sorte : Roi | K \n";
    cout << "                                                      Reine | Q \n";
    cout << "                                                      Fou | B \n";
    cout << "                                                      Cavalier | N \n";
    cout << "                                                      Tour | R \n";
    cout << "                                                      Pion | P \n";
    cout << " Les pieces feeriques de capablanca sont representes de la sorte : Cardinal | C \n";
    cout << "                                                                   Marechal | M \n\n";

    cout << " Une piece blanche est ecrite en noir sur fond blanc comme ceci : ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF0);
    cout << " R ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    cout << "\n Une piece noire est ecrite en blanc sur fond noir comme ceci : ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    cout << " R ";
    cout << "\n\n";

    cout << " Losque vous selectionnerez une piece, celle-ci s'affichera sur fond vert : ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x2F);
    cout << " R ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    cout << "\n";
    cout << " Losque la piece selectionne peut se deplacer sur une case vide, la case vide s'affichera en bleu : ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x11);
    cout << "   ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    cout << "\n";
    cout << " Losque la piece selectionne peut prendre une piece adverse, la piece adverse s'affichera en rouge : ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4F);
    cout << " R ";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0F);
    cout << "\n\n\n";


    // Présentation des variantes jouables
    cout << "II. Variantes jouables\n\n";
    cout << " - Variante de Capablanca : les regles sont les memes, a l'exception de l'ajout de 2 pieces :\n";
	cout << "\t-> le marechal, qui peut se deplacer comme un cavalier et une tour\n";
	cout << "\t-> le cardinal, qui peut se deplacer comme un cavalier et un fou.\n\n";

    cout << " - Variante de Karnaugh : les regles sont les memes, mais l'echiquier est un cylindre,\n   les bords droits et gauches sont relies, une piece peut passer d'un bord a l'autre.\n\n\n";
}



// Main
int main(int argc, char *argv[]) {
    // Configuration de la console, couleur du texte et taille du texte
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 21;
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

    std::set_terminate(&mon_gestionnaire);
    
    mainMenu();
    
    return 0;
}
