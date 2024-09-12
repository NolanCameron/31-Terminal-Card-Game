#pragma once
#ifndef GAME_HH
#define GAME_HH
#include "31Menu.hh"
#include "board.hh"
#include "player.hh"
#include "rectangleArea.h"
#include <vector>
#include <ncursesw/curses.h>
#include <memory>

class Game{
    private:

        Menu menu;
        int round;
        int cols, rows;
        int numberOfOpponents;
        bool running;
        rectangleArea playSpace;
        rectangleArea handSpace;
        Player mainPlayer;
        std::vector<Opponent> opponents;
        std::unique_ptr<Board> board;
        
    public:
        Game();
        void start();
        bool isRunning();
        void endGame();
        void startRound();

    private:
        int playerChooseDraw();
        int playerChooseCard();
        void takeTurns(int&, int&);
        void judgeRound(int, int);
        void clearPlayerHands();
};

#endif