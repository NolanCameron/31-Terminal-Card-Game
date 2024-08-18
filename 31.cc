#include <locale.h>
#include <iostream>
#include <ncursesw/ncurses.h>
#include "31.h"
#include "rectangleArea.h"


rectangleArea playSpace(14,68,0,1);
rectangleArea handSpace(playSpace.height - 10,playSpace.width - 34,playSpace.y + 14,playSpace.x + 17);

class Game{

};

int main(int argc, char ** argv){

    setlocale(LC_ALL, "");
    initscr();
    curs_set(0);

    Board board(5, playSpace, handSpace);
    
    getch();

    return 0;
}
