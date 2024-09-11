#pragma once
#ifndef BOARD_HH
#define BOARD_HH
#include <vector>
#include "coord.h"
#include "rectangleArea.h"
#include <random>
#include <algorithm>
#include "card.h"
#include <ncursesw/curses.h>
#include "player.hh"

class Player;

class Opponent;

class Board{

    private:
        int numPlayers;
        std::vector<Card> deck;
        std::vector<Card> discard;
        std::vector<Opponent>& opponents;
        Player& mainPlayer;
        WINDOW* playWindow;
        WINDOW* handWindow;
        WINDOW* promptWindow;
        Coord playSelectorPosition;
        Coord handSelectorPosition;
        rectangleArea playSpace;
        rectangleArea handSpace;
        const Coord deckPosition;
        const Coord discardPosition;
        friend class Player;
        friend class Opponent;

    public:
        Board(int, rectangleArea, rectangleArea, Player&, std::vector<Opponent>&);
        ~Board();
        void movePlaySelector(Coord);
        void moveHandSelector(Coord);
        void drawHand();
        void drawBoard();
        void deal(int);
        void shuffle();
        void drawOpponentHands();
        void drawPlayers();
        void movePlaySelector(int);
        void moveHandSelector(int);
        void removePlaySelector();
        void removeHandSelector();
        void knockPrompt(int);
        void roundWinPrompt(int);
        void continuePrompt();
        void clearPromptWin();
        void displayPlayerScore();
        void playerKnockPrompt(int);
        void knockPrompt();
        void clearBoard();
    private:
        void initDeck();
};

#endif