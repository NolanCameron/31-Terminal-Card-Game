#pragma once
#ifndef PLAYER_HH
#define PLAYER_HH
#include <vector>
#include "coord.h"
#include <ncursesw/curses.h>
#include "rectangleArea.h"
#include "card.h"
#include <algorithm>
#include "board.h"

class Board;

class Player{

    protected:

        std::vector<Card> handCards;
        std::vector<Coord> cardPositions;
        int score;

        friend class Board;

    public:

        Player(int, int);
        void drawFromDeck(Board&);
        void drawFromDiscard(Board&);
        void discardCard(Board&, int);
        float getScore();
        float calculateScore();
};

class Opponent:Player{

    private:

        void chooseDraw(Board&);
        void chooseDiscard(Board&);
        float calculateHand(std::vector<Card>);

    public:

        void makeMove(int, Board&);

};
#endif