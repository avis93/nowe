#include <iostream>
#include "C:/Projekt/gameLib/gra.h"
#include "C:/Projekt/gameLib/libgame.h"

using namespace std;


int main() {
    Board board;
    Player *player;
    char choose;

    do {
        board.clear();
        cout << "Gra w Kolko i Krzyzyk dla gracza i komputera\n"
                "============================================\n\n";
        player = human;

        while (!won(&board, ai, false) && !won(&board, human, false) && !drawGame(&board, false)) {
            player = gameMove(&board, player);
        }

        cout << "Jeszcze raz ? (T = TAK) : ";
        cin >> choose;
        cout << "\n\n\n";

    } while ((choose == 'T') || (choose == 't'));
}
