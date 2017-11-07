#ifndef gra_h
#define gra_h
#include <iostream>

using namespace std;

const int SIZE = 9;

#define TIC 'O'
#define TAC 'X'
#define EMPTY ' '

enum PlayerName {
    AI,
    HUMAN
};

class Player {
public:
    char character;
    PlayerName name;
protected:
    Player() {};
};

class Ai : public Player {
public:
    Ai() {
        character = TIC;
        name = AI;
    }
};

class Human : public Player {
public:
    Human() {
        character = TAC;
        name = HUMAN;
    }
};

class Board {
public:
    char t[SIZE + 1];

    Board() {
        clear();
    }

    void clear(){
        for (int i = 1; i <= SIZE; i++) {
            t[i] = EMPTY;
        }
    }

    void draw() {
        for (int i = 1; i <= SIZE; i++) {
            cout << " " << t[i] << " ";
            if (i % 3 != 0) {
                cout << "|";
            } else if (i != SIZE) {
                cout << "\n---+---+---\n";
            } else {
                cout << endl;
            }
        }
    }
};

#endif
