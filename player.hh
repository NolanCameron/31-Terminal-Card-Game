#pragma once
#ifndef PLAYER_HH
#define PLAYER_HH
#include <vector>
#include "coord.hh"
#include <ncursesw/curses.h>
#include "rectangleArea.hh"
#include "card.h"
#include <algorithm>
#include "board.hh"

class Board;

class Player{

    protected:

        std::vector<Card> handCards; //Cards in Hand
        std::vector<Coord> cardPositions; //Card Positions on the Board
        int points; //Point Value of Hand
        bool knockVal = 0; //1 => player has knocked, 0 => else

        friend class Board;

    public:

        Player(int y, int x); //Creates player at position (y, x)
        void drawFromDeck(Board&); //Adds to the players hand the top card of the deck
        void drawFromDiscard(Board&); //Adds to the players hand the top card of the discard
        void discardCard(Board&, int index); //discard card at index
        int getPoints(); //Returns value of points
        float calculateScore(); //Calculates Players Score
        int handSize(); //Returns size of handCards
        bool knocked(); //Returns knockVal
        bool is31(); //Returns true if score is 31
        void knock(); //Sets knockVal to true
        void addPoints(int points); //increments the points of the Player
        void clearHand(); //Empties handCards
        void resetKnock(); //Sets knockVal to false
};

class Opponent: public Player{

    private:

        using Player::Player;
        void chooseDraw(Board&);
        void chooseDiscard(Board&);
        float calculateHand(std::vector<Card>);
        bool chooseKnock(int, bool);

        friend class Board;

    public:

        void makeMove(int, bool&, Board&);


};
#endif