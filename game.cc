#include "game.hh"
#include "31Menu.hh"
#include "card.h"

Game::Game() : 
menu(Menu(getmaxy(stdscr),getmaxx(stdscr))),
running(false),
playSpace(rectangleArea(14, 68, 0, 1)),
handSpace(rectangleArea(5, playSpace.width*2/3, playSpace.y + playSpace.height, playSpace.width/6)),
mainPlayer(Player(handSpace.height/2,(handSpace.width-6)/2))
{

}

void Game::start(){

    if(running == true)
        return;

    keypad(stdscr, true);

    running = true;

    menu.start();

    numberOfOpponents = menu.getNumberOfOpponents();

    round = menu.getRounds();

    for(int i = 0; i < numberOfOpponents; ++i)
        opponents.push_back(Opponent(2, (playSpace.width - 11*numberOfOpponents)/(numberOfOpponents + 1)*(i+1) + 11*(i) + 2));

    board = std::make_unique<Board>(numberOfOpponents, playSpace, handSpace, mainPlayer, opponents);

    while(round != 0){
        startRound();
        --round;
    }

    endGame();

    getch();

    running = false;

}

void Game::startRound(){

    int got31OrKnock;
    int index;
    
    takeTurns(got31OrKnock, index);

    board->clearPromptWin();
    board->roundOverPrompt();

    judgeRound(got31OrKnock, index);

    board->revealOpponentsHands();

    getch();

    resetPlayers();

    board->resetBoard();

}

void Game::takeTurns(int& got31OrKnock, int& index){

    int turn = 0;
    bool playerHasKnocked = false;

    board->displayPlayerScore();

    while(1){

        if(mainPlayer.knocked()){
            board->playerKnockPrompt(0);
            got31OrKnock = 1;
            index = 0;
            return;
        }

        board->knockPrompt();

        int k = getch();
        if(k == 'k' || k == 'K'){
            mainPlayer.knock();
            board->playerKnockPrompt(0);
            playerHasKnocked = true;
        }else{

            board->clearPromptWin();

            if(playerChooseDraw() == 1)
                mainPlayer.drawFromDiscard(*board);
            else
                mainPlayer.drawFromDeck(*board);

            board->drawHand();

            board->drawBoard();

            mainPlayer.discardCard(*board, playerChooseCard());

            board->drawBoard();

            board->displayPlayerScore();

            board->drawHand();

            if(mainPlayer.is31()){
                board->roundWinPrompt(0);
                got31OrKnock = 0;
                index = 0;
                return;
            }

            board->continuePrompt();


        }

        ++turn;

        getch();

        for(Opponent& opponent: opponents){

            if(opponent.knocked()){
                got31OrKnock = 1;
                index = turn % (numberOfOpponents + 1);
                return;
            }

            opponent.makeMove(turn, playerHasKnocked, *board);

            if(opponent.knocked())
                board->playerKnockPrompt(turn % (numberOfOpponents + 1));

            if(opponent.is31()){
                board->roundWinPrompt(turn % (numberOfOpponents + 1));
                got31OrKnock = 0;
                index = turn % (numberOfOpponents + 1);
                return;
            }

            ++turn;
        }

        board->clearPromptWin();

    }
}

int Game::playerChooseDraw(){

    int input;
    int index = 0;

    board->movePlaySelector(index);

    while(1){
        input = getch();
        switch (input)
        {
        case KEY_RIGHT:
            index = 1;
            break;

        case KEY_LEFT:
            index = 0;
            break;
        
        case '\n':
            board->removePlaySelector();
            return index;


        default:
            continue;
            break;
        }
        board->movePlaySelector(index);
    }

    return index;
}

int Game::playerChooseCard(){

        int input;
        int index = 0;

        board->moveHandSelector(index);

        while(1){
            input = getch();
            switch (input)
            {
            case KEY_RIGHT:
                if(index == (int)mainPlayer.handSize() - 1)
                    continue;
                ++index;
                break;

            case KEY_LEFT:
                if(index == 0)
                    continue;
                --index;
                break;

            case '\n':
                board->removeHandSelector();
                return index;
                break;
            
            default:
                continue;
                break;
            }
        board->moveHandSelector(index);
    }
}

void Game::judgeRound(int got31OrKnocked, int index){

    if(got31OrKnocked == 0){

        for(Opponent& opponent: opponents)
            opponent.addPoints(1 + opponent.knocked());

        if(index != 0){
            mainPlayer.addPoints(1 + mainPlayer.knocked());
            opponents.at(index - 1).addPoints(-2);
        }

    }else{

        float lowestScore = mainPlayer.calculateScore();
        for(Opponent& opponent: opponents){
            float k = opponent.calculateScore();
            if(k < lowestScore)
                lowestScore = k;
        }

        if(mainPlayer.calculateScore() == lowestScore)
            mainPlayer.addPoints(1 + mainPlayer.knocked());

        for(Opponent& opponent: opponents){
            if(opponent.calculateScore() == lowestScore)
                opponent.addPoints(1 + opponent.knocked());

        }

    }
}

void Game::resetPlayers(){
    mainPlayer.resetKnock();
    mainPlayer.clearHand();
    for(Opponent& opponent : opponents){
        opponent.clearHand();
        opponent.clearHand();
    }

}

void Game::endGame(){
    board->clearBoard();
    board->printResults();
}