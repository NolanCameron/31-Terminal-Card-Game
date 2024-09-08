#include <locale.h>
#include <iostream>
#include <ncursesw/ncurses.h>
#include "31.h"
#include "rectangleArea.h"
#include "game.hh"

#include "board.hh"

int main(int argc, char ** argv){

    setlocale(LC_ALL, "");
    initscr();
    curs_set(0);
    noecho();

    //rectangleArea playSpace(14, 68, 0, 1);
    //rectangleArea handSpace(4, playSpace.width/2, playSpace.y + 14,playSpace.x + 17);

    //Board board(4, playSpace, handSpace);

    Game game;

    game.start();

    getch();

    return 0;
}
