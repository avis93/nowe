#define libgame_h
#include "gra.h"
#include <iostream>

Player *human = new Human();
Player *ai = new Ai();

bool won(Board *board, Player* player, bool wait) {
    bool test;

    test = false; // true jeśli player ma 03 figury w kolumnie, wierszu lub na przekątnej
// Sprawdza wiersze

    for (int i = 1; i <= 7; i += 3) {
        test |= ((board->t[i] == player->character) && (board->t[i + 1] == player->character) && (board->t[i + 2] == player->character));
    }

// Sprawdza kolumny

    for (int i = 1; i <= 3; i++) {
        test |= ((board->t[i] == player->character) && (board->t[i + 3] == player->character) && (board->t[i + 6] == player->character));
    }

// Sprawdza przek¹tn¹ 1-5-9

    test |= ((board->t[1] == player->character) && (board->t[5] == player->character) && (board->t[9] == player->character));

// Sprawdza przek¹tn¹ 3-5-7

    test |= ((board->t[3] == player->character) && (board->t[5] == player->character) && (board->t[7] == player->character));

    if (test) {
        if (!wait) {
            board->draw();
            cout << "\nGRACZ " << player->name << " WYGRYWA!!!\n\n";
        }
        return true;
    }
    return false;
}

// true jeśli wszystkie pola są zajęte (wole pole to spacja)

bool drawGame(Board *board, bool wait) {
    for (int i = 1; i <= SIZE; i++) {
        if (board->t[i] == EMPTY) {
            return false;
        }
    }

    if (!wait) {
        board->draw();
        cout << "\nREMIS !!!\n\n";
    }
    return true;
}

// Algorytm rekurencyjny MINIMAX
int minimax(Board *board, Player* player) {
    int m, mmx;
    if (won(board, player, true)) {
        return (player->name == AI) ? 1 : -1;
    }
    if (drawGame(board, true)) {
        return 0;
    }
    player = (player->name == AI) ? human : ai;
    mmx = (player->name == HUMAN) ? 10 : -10;
    for (int i = 1; i <= SIZE; i++) {
        if (board->t[i] == EMPTY) {
            board->t[i] = player->character;
            m = minimax(board, player);
            board->t[i] = EMPTY;
            if (((player->name == HUMAN) && (m < mmx)) || ((player->name == AI) && (m > mmx))) mmx = m;
        }
    }
    return mmx;
}

// Funkcja wyznacza ruch dla aia.
int aiMove(Board *board) {
    int gameMove, m, mmx;

    mmx = -10;
    for (int i = 1; i <= 9; i++) {
        if (board->t[i] == EMPTY) {
            board->t[i] = ai->character;
            m = minimax(board, ai);
            board->t[i] = EMPTY;
            if (m > mmx) {
                mmx = m;
                gameMove = i;
            }
        }
    }
    return gameMove;
}

// Funkcja umo¿liwia ruch gracza
// Po ruchu nastêpuje zamiana gracza
//------------------------------------
Player* gameMove(Board *board, Player *player) {
    int r;

    board->draw();
    if (player->name == HUMAN) {
            cout << "\nCZLOWIEK : wybiera ruch : ";
        do {
            cin >> r;
            if(board->t[r] != EMPTY){
                cout << "To pole jest juz zajete. Wybierz jeszcze raz:";
            } else{
                break;
            }
        } while(true);
    } else {
        r = aiMove(board);
        cout << "\nKOMPUTER : wybiera ruch : " << r << endl;
    }
    cout << "---------------------------\n\n";
    if ((r >= 1) && (r <= 9) && (board->t[r] == EMPTY)) {
        board->t[r] = player->character;
    }
    return (player->name == HUMAN) ? ai : human;
}
