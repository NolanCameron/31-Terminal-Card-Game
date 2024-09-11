#pragma once
#ifndef MENU31_HH
#define MENU31_HH

#include <ncursesw/ncurses.h>
#include<fstream>

class Menu{

    private:
        int y, x;
        int numberOfRounds;
        int numberOfOpponents;
        std::ifstream rulesFile;
        const int maxOpponents = 6;
        const int maxRounds = 9;

    public:
        Menu(int,int);
        ~Menu();
        void start();
        int getNumberOfOpponents();
        int getYSize();
        int getXSize();
        int getRounds();

    private:
        void printRules();
        void getInitVals();
        void setGameSize();
        void setSettings();

};

#endif