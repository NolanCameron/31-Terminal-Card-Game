#include "board.h"

const char* selector = "⮝";

Board::Board(int amountOfPlayers, rectangleArea playSpace, rectangleArea handSpace): 
    numPlayers(amountOfPlayers),  
    playWindow(newwin(playSpace.height, playSpace.width, playSpace.y, playSpace.x)),
    handWindow(newwin(handSpace.height, handSpace.width, handSpace.y, handSpace.x)), 
    playSelectorPosition(Coord()),
    handSelectorPosition(Coord()),
    playSpace(playSpace),
    handSpace(handSpace),
    deckPosition(Coord(playSpace.height/2,playSpace.width/2-1)),
    discardPosition(Coord(playSpace.height/2,playSpace.width/2+1))
{
        initDeck();
        players.push_back(Player(handSpace.height/2-1,(handSpace.width-6)/2));
        for(int i = 1; i < numPlayers; ++i){players.push_back(Player(2,playSpace.width/(numPlayers*2-1)*(2*i-1)));}

        refresh();
        box(playWindow,0,0);
        box(handWindow,0,0);
        wprintw(playWindow,"31");
        wprintw(handWindow,"Hand");

        shuffle();
        deal(3);
        drawPlayers();
        drawBoard();
        drawHand();
        drawOpponentHands();

        wrefresh(playWindow);
        wrefresh(handWindow);

        wgetch(handWindow);
        wgetch(playWindow);

}

void Board::initDeck(){
    for(int i = 0; i < 4; ++i) for(int j = 0; j < 13; ++j){
            deck.push_back({suite(i),face(j),symbols[i*13+j],j>10 ? 10 : (j == 1 ? 11 : j)});
        }
}

void Board::drawPlayers(){

    for(int i = 1; i < (int)players.size(); ++i){
        mvwprintw(playWindow,1,playSpace.width/(players.size()*2-1)*(i*2-1),"Player %d",i);
    } 

}

void Board::drawOpponentHands(){
    for(int i = 1; i < (int)players.size(); ++i){
        for(int j = 0; j < (int)players.at(i).handCards.size(); ++j){
            mvwprintw(playWindow, players.at(i).cardPositions.at(j).y, players.at(i).cardPositions.at(j).x, cardBack);
        }
    } 
}

void Board::shuffle(){

    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(deck), std::end(deck), rng);
    
}

void Board::deal(int amountCards){

    for(int j = 0; j < amountCards; ++j) for(int i = 0; i < (int)players.size(); ++i){
        players.at(i).drawFromDeck(*this);
    }

    discard.push_back(deck.back());
    deck.pop_back();

}

void Board::drawBoard(){

    if(deck.size() > 0) mvwprintw(playWindow, deckPosition.y, deckPosition.x, cardBack);
    if(discard.size() != 0) {mvwprintw(playWindow, discardPosition.y, discardPosition.x, discard.back().sym);}
    else{mvwdelch(playWindow, discardPosition.y, discardPosition.x);}

}

void Board::drawHand(){

    for(int i = 0; i < (int)players.at(0).handCards.size(); ++i) mvwprintw(handWindow, players.at(0).cardPositions.at(i).y, players.at(0).cardPositions.at(i).x, players.at(0).handCards.at(i).sym);

}

Board::~Board(){

        delwin(playWindow);
        delwin(handWindow);
        endwin();

}

void Board::moveHandSelector(Coord Position){
        mvwdelch(playWindow,handSelectorPosition.y, handSelectorPosition.x);
        handSelectorPosition = Position;
        mvwprintw(playWindow, handSelectorPosition.y, handSelectorPosition.x, selector);
}

void Board::movePlaySelector(Coord Position){
        mvwdelch(playWindow,playSelectorPosition.y, playSelectorPosition.x);
        playSelectorPosition = Position;
        mvwprintw(playWindow, playSelectorPosition.y, playSelectorPosition.x, selector);
}
