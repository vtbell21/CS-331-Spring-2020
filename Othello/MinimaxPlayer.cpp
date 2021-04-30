/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 */
#include <iostream>
#include <assert.h>
#include <algorithm>
#include <climits>
#include "MinimaxPlayer.h"

using std::vector;

MinimaxPlayer::MinimaxPlayer(char symb) :
    Player(symb) {

}

MinimaxPlayer::~MinimaxPlayer() {

}

// Minimax decision function
// Made for human as player 1 and minimax and player 2
void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {

    std::vector<OthelloBoard*> child = succ(*b, get_symbol());
    int high = INT_MAX;
    int r = 0;
    int c = 0;
    int value;

    for (int i = 0; i < child.size(); i++) {
        value = Max_Value(*child[i]);
        if (value < high) {
            high = value;
            r = child[i]->get_row();
            c = child[i]->get_col();
        }
    }
    row = r;
    col = c;
}

int MinimaxPlayer::Max_Value(OthelloBoard b) {
    int maxval = INT_MIN;
    char player = b.get_p1_symbol();
    std::vector<OthelloBoard*> child;
    child = succ(b, player);

    if (Terminal(b) == true) {
        return Utility(b);
    }
    for (int i = 0; i < child.size(); i++) {
        if (Min_Value(*child[i]) > maxval) {
            maxval = Min_Value(*child[i]);
        }
    }
    return maxval;
}

int MinimaxPlayer::Min_Value(OthelloBoard b) {
    int minval = INT_MAX;
    char player = b.get_p1_symbol();
    std::vector<OthelloBoard*> child;
    child = succ(b, player);

    if (Terminal(b) == true) {
        return Utility(b);
    }
    for (int i = 0; i < child.size(); i++) {
        if (Max_Value(*child[i]) < minval) {
            minval = Max_Value(*child[i]);
        }
    }
    return minval;
}

// Gets successors
std::vector<OthelloBoard*> MinimaxPlayer::succ(OthelloBoard b, char player) {
    std::vector<OthelloBoard*> board;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (b.is_legal_move(j, i, player)) {
                OthelloBoard* newb = new OthelloBoard(b);
                newb->play_move(j, i, player);
                newb->set_row(i);
                newb->set_col(j);
                board.push_back(newb);
            }
        }
    }
    return board;
}

// Utility value is the difference between the scores
int MinimaxPlayer::Utility(OthelloBoard b) {
    int UtilityValue = b.count_score(b.get_p1_symbol()) - b.count_score(b.get_p1_symbol());

    return UtilityValue;
}

// Terminal test that determines when the game ends
bool MinimaxPlayer::Terminal(OthelloBoard b) {
    if ((b.has_legal_moves_remaining(b.get_p1_symbol())) && !(b.has_legal_moves_remaining(b.get_p2_symbol()))) {
        return true;
    }

    return false;
}

MinimaxPlayer* MinimaxPlayer::clone() {
    MinimaxPlayer* result = new MinimaxPlayer(symbol);
    return result;
}


