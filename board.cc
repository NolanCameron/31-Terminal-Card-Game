#include <board.h>
#include <random>
#include <algorithm>
#include <ncursesw/curses.h>
const char* selector = "⮝";

Board::Board(int numPlayers, rectangleArea playSpace, rectangleArea handSpace): 
    play(newwin(playSpace.height, playSpace.width, playSpace.y, playSpace.x)),
    hand(newwin(handSpace.height, handSpace.width, handSpace.y, handSpace.x)), 
    numPlayers(numPlayers), handSelectorPosition(Coord(0,0)), 
    playSelectorPosition(Coord()),
    playSpace(playSpace),
    handSpace(handSpace),
    deckPosition(Coord(playSpace.height/2,playSpace.width/2-1)),
    discardPosition(Coord(playSpace.height/2,playSpace.width/2+1))
{

        initDeck();
        players.push_back(Player(handSpace.height/2-1,(handSpace.width-6)/2));
        for(int i = 1; i < numPlayers; ++i){players.push_back(Player(2,playSpace.width/(numPlayers*2-1)*(2*i-1)));}

        refresh();
        box(play,0,0);
        box(hand,0,0);
        wprintw(play,"31");
        wprintw(hand,"Hand");

        shuffle();
        deal(3);
        drawPlayers();
        drawBoard();
        drawHand();
        drawOpponentHands();

        wrefresh(play);
        wrefresh(hand);

        wgetch(hand);
        wgetch(play);

}

void Board::initDeck(){
    for(int i = 0; i < 4; ++i) for(int j = 0; j < 13; ++j){
            deck.push_back({suite(i),face(j),sym[i*13+j],j>10 ? 10 : j});
        }
}

void Board::drawPlayers(){

    for(int i = 1; i < players.size(); ++i){
        mvwprintw(play,1,playSpace.width/(players.size()*2-1)*(i*2-1),"Player %d",i);
    } 

}

void Board::drawOpponentHands(){
    for(int i = 1; i < players.size(); ++i){
        for(int j = 0; j < players.at(i).handCards.size(); ++j){
            mvwprintw(play, players.at(i).cardPositions.at(j).y, players.at(i).cardPositions.at(j).x, cardBack);
        }
    } 
}

void Board::shuffle(){

    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(deck), std::end(deck), rng);
    
}

void Board::deal(int amountCards){

    for(int j = 0; j < amountCards; ++j) for(int i = 0; i < players.size(); ++i){
        players.at(i).drawFromDeck(*this);
    }

    discard.push_back(deck.back());
    deck.pop_back();

}

void Board::drawBoard(){

    if(deck.size() > 0) mvwprintw(play, deckPosition.y, deckPosition.x, cardBack);
    if(discard.size() != 0) {mvwprintw(play, discardPosition.y, discardPosition.x, discard.back().sym);}
    else{mvwdelch(play, discardPosition.y, discardPosition.x);}

}

void Board::drawHand(){

    for(int i = 0; i < players.at(0).handCards.size(); ++i) mvwprintw(hand, players.at(0).cardPositions.at(i).y, players.at(0).cardPositions.at(i).x, players.at(0).handCards.at(i).sym);

}

Board::~Board(){

        delwin(play);
        delwin(hand);
        endwin();

}

void Board::moveHandSelector(Coord Position){
        mvwdelch(play,handSelectorPosition.y, handSelectorPosition.x);
        handSelectorPosition = Position;
        mvwprintw(play, handSelectorPosition.y, handSelectorPosition.x, selector);
}

void Board::movePlaySelector(Coord Position){
        mvwdelch(play,playSelectorPosition.y, playSelectorPosition.x);
        playSelectorPosition = Position;
        mvwprintw(play, playSelectorPosition.y, playSelectorPosition.x, selector);
}