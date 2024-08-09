#pragma once
#include <vector>
#include <ncursesw/curses.h>
#include <coord.h>
#include <player.h>
#include <card.h>
#include <rectangleArea.h>

class Board{

    private:
        int numPlayers;
        std::vector<Card> deck;
        std::vector<Card> discard;
        std::vector<Player> players;
        WINDOW *play;
        WINDOW *hand;
        Coord playSelectorPosition;
        Coord handSelectorPosition;
        rectangleArea playSpace;
        rectangleArea handSpace;
        Coord deckPosition;
        Coord discardPosition;

        friend class Player;

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