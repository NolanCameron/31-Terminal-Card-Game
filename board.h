#pragma once
#include <vector>
#include "coord.h"
#include "rectangleArea.h"
#include <random>
#include <algorithm>
#include "card.h"
#include <ncursesw/curses.h>
#include "player.h"

class Player;

class Board{

    private:
        int numPlayers;
        std::vector<Card> deck;
        std::vector<Card> discard;
        std::vector<Player> players;
        WINDOW *playWindow;
        WINDOW *handWindow;
        Coord playSelectorPosition;
        Coord handSelectorPosition;
        rectangleArea playSpace;
        rectangleArea handSpace;
        Coord deckPosition;
        Coord discardPosition;

        friend class Player;
        friend class Opponent;

    public:
        Board(int, rectangleArea, rectangleArea);
        ~Board();
        void movePlaySelector(Coord);
        void moveHandSelector(Coord);
        void drawHand();
        void drawBoard();
        void deal(int);
        void shuffle();
        void drawOpponentHands();
        void drawPlayers();
    private:
        void initDeck();
};