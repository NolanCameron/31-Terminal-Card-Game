#include "31Menu.hh"
#include <fstream>
#include <vector>
#include <string>

Menu::Menu(int rows, int cols) : y(rows), x(cols), numberOfRounds(0), numberOfOpponents(0){
    rulesFile.open("31rules.txt");
}

Menu::~Menu(){
    rulesFile.close();
}

void Menu::printRules(){

    int maxLineLength = x - 5;
    std::string str;
    std::string line;
    int rowPosition = 1;
    
    move(rowPosition,2);
    while(std::getline(rulesFile,str)){
        if((int)line.length() + (int)str.length() + 1 > maxLineLength){
            printw("%s ",line.c_str());
            move(++rowPosition,2);
            line.clear();
        }
            line += " " + str;
        
    }
    printw("%s ", line.c_str());

}

void Menu::setSettings(){

    int k;

    mvprintw(1,2,"How many other players do you want to play with?");
    mvprintw(3,4,"Type a number between 1-5");

    k = getch();

    while(k < '1' || k >= '0' + maxOpponents){
        mvprintw(3,4,"Please type a number between 1-5");
        k = getch();    
    }

    numberOfOpponents = k - '0';

    clear();

    mvprintw(1,2,"How many rounds do you want to play?");
    mvprintw(3,4,"Type a number between 1-9");

    k = getch();

    while(k < '1' || k > '0' + maxRounds){
        mvprintw(3,4,"Please type a number between 1-9");
        k = getch();    
    }

    numberOfRounds = k - '0';

}

void Menu::start(){

    /*printRules();

    printw("\n\n\tHit any key to contiune");

    getch();
    */
    clear();

    setSettings();

    clear();

}

int Menu::getXSize(){
    return x;
}

int Menu::getYSize(){
    return y;
}

int Menu::getNumberOfOpponents(){
    return numberOfOpponents;
}

int Menu::getRounds(){
    return numberOfRounds;
}