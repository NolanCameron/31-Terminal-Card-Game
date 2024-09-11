#pragma once
#ifndef PLAYER_HH
#define PLAYER_HH
#include <vector>
#include "coord.h"
#include <ncursesw/curses.h>
#include "rectangleArea.h"
#include "card.h"
#include <algorithm>
#include "board.hh"

class Board;

class Player{

    protected:

        std::vector<Card> handCards;
        std::vector<Coord> cardPositions;
        int points;
        bool knockVal;

        friend class Board;

    public:

        Player(int, int);
        void drawFromDeck(Board&);
        void drawFromDiscard(Board&);
        void discardCard(Board&, int);
        int getPoints();
        float calculateScore();
        int handSize();
        bool knocked();
        bool is31();
        void knock();
};

class Opponent: public Player{

    private:

        using Player::Player;
        void chooseDraw(Board&);
        void chooseDiscard(Board&);
        float calculateHand(std::vector<Card>);
        bool chooseKnock(int);

        friend class Board;

    public:

        void makeMove(int, Board&);


};
#endif