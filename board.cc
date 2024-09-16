#include "board.hh"
#include <string>

const char* selector = "⮝";

Board::Board(int amountOfPlayers, rectangleArea playSpace, rectangleArea handSpace, Player& player, std::vector<Opponent>& opponentVector): 
    numPlayers(amountOfPlayers),
    mainPlayer(player),
    opponents(opponentVector),
    playWindow(newwin(playSpace.height, playSpace.width, playSpace.y, playSpace.x)),
    handWindow(newwin(handSpace.height, handSpace.width, handSpace.y, handSpace.x)), 
    promptWindow(newwin(1, playSpace.width - 2, playSpace.height - 2, playSpace.x + 1)),
    playSpace(playSpace),
    handSpace(handSpace),
    deckPosition(Coord(playSpace.height/2,playSpace.width/2-1)),
    discardPosition(Coord(playSpace.height/2,playSpace.width/2+1)),
    handSelectorPosition(Coord())
{
        playSelectorPosition = (Coord(deckPosition.y + 1, deckPosition.x));

        initDeck();
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

}

void Board::initDeck(){
    for(int i = 0; i < 4; ++i) for(int j = 0; j < 13; ++j){
            deck.push_back({suite(i), face(j), symbols[i*13+j], j + 1 > 10 ? 10 : (j + 1 == 1 ? 11 : j + 1)});
        }
}

void Board::drawPlayers(){

    for(int i = 0; i < (int)opponents.size(); ++i){
        mvwprintw(playWindow, 1, opponents.at(i).cardPositions.at(0).x - 2,"Player %d: %d", i + 1, opponents.at(i).getPoints());
    } 

}

void Board::drawOpponentHands(){

    for(Opponent opponent : opponents)
        for(int i = 0; i < (int)opponent.cardPositions.size(); ++i) 
            if(i < (int)opponent.handCards.size())
                mvwprintw(playWindow, opponent.cardPositions.at(i).y, opponent.cardPositions.at(i).x, cardBack);
            else
                mvwaddch(playWindow, opponent.cardPositions.at(i).y, opponent.cardPositions.at(i).x, ' ');

    wrefresh(playWindow);

}

void Board::shuffle(){

    auto rd = std::random_device {}; 
    auto rng = std::default_random_engine { rd() };
    std::shuffle(std::begin(deck), std::end(deck), rng);
    
}

void Board::deal(int amountCards){

    for(int j = 0; j < amountCards; ++j){
        mainPlayer.drawFromDeck(*this);
        for(int i = 0; i < (int)opponents.size(); ++i)
            opponents.at(i).drawFromDeck(*this);
    }

    discard.push_back(deck.back());
    deck.pop_back();

}

void Board::drawBoard(){

    if(deck.size() > 0) mvwprintw(playWindow, deckPosition.y, deckPosition.x, cardBack);
    if(discard.size() != 0) {mvwprintw(playWindow, discardPosition.y, discardPosition.x, discard.back().sym);}
    else{mvwaddch(playWindow, discardPosition.y, discardPosition.x, ' ');}

    wrefresh(playWindow);
}

void Board::drawHand(){

    for(int i = 0; i < (int)mainPlayer.cardPositions.size(); ++i) 
        if(i < (int)mainPlayer.handCards.size())
            mvwprintw(handWindow, mainPlayer.cardPositions.at(i).y, mainPlayer.cardPositions.at(i).x, mainPlayer.handCards.at(i).sym);
        else
            mvwaddch(handWindow, mainPlayer.cardPositions.at(i).y, mainPlayer.cardPositions.at(i).x, ' ');


    wrefresh(handWindow);

}

Board::~Board(){

        delwin(playWindow);
        delwin(handWindow);
        delwin(promptWindow);
        endwin();

}

void Board::moveHandSelector(int position){

        if(playSelectorPosition.y != -1)
            mvwaddch(handWindow, handSelectorPosition.y, handSelectorPosition.x, ' ');
        handSelectorPosition = Coord(mainPlayer.cardPositions.at(position).y + 1, mainPlayer.cardPositions.at(position).x);
        mvwprintw(handWindow, handSelectorPosition.y, handSelectorPosition.x, selector);
        wrefresh(handWindow);
}

void Board::movePlaySelector(int position){

        mvwaddch(playWindow, playSelectorPosition.y, playSelectorPosition.x, ' ');
        if(position == 0)
            playSelectorPosition.x = deckPosition.x;
        else
            playSelectorPosition.x = discardPosition.x;
        mvwprintw(playWindow, playSelectorPosition.y, playSelectorPosition.x, selector);
        wrefresh(playWindow);
}



void Board::removePlaySelector(){
    mvwaddch(playWindow, playSelectorPosition.y, playSelectorPosition.x, ' ');
    wrefresh(playWindow);

}

void Board::removeHandSelector(){
    mvwaddch(handWindow, handSelectorPosition.y, handSelectorPosition.x, ' ');
    wrefresh(handWindow);
}

void Board::playerKnockPrompt(int index){
    
    wclear(promptWindow);
    const std::string mainPlayerKnocked = "You Knocked!";
    const std::string opponentKnocked = "Player %d Knocked!";

    if(index == 0)
        mvwprintw(promptWindow, 0, (playSpace.width - mainPlayerKnocked.length())/2, mainPlayerKnocked.c_str());
    else
        mvwprintw(promptWindow, 0, (playSpace.width - opponentKnocked.length())/2, opponentKnocked.c_str(), index);
    
    wrefresh(promptWindow);

}

void Board::knockPrompt(){
    
    wclear(promptWindow);
    const std::string knockedTxt = "Press k to Knock Else, Hit Any Key";

    mvwprintw(promptWindow, 0, (playSpace.width - knockedTxt.length())/2, knockedTxt.c_str());
    
    wrefresh(promptWindow);

}

void Board::roundWinPrompt(int index){

    wclear(promptWindow);
    const std::string mainPlayerKnocked = "You Got 31! You Win!";
    const std::string opponentKnocked = "Player %d Got 31! They Win!";

    if(index == 0)
        mvwprintw(promptWindow, 0, (playSpace.x - mainPlayerKnocked.length())/2, mainPlayerKnocked.c_str());
    else
        mvwprintw(promptWindow, 0, (playSpace.x - opponentKnocked.length())/2, opponentKnocked.c_str(), index + 1);
    
    wrefresh(promptWindow);
}

void Board::continuePrompt(){

    wclear(promptWindow);
    const std::string continueTxt = "Press any key to Continue";

    mvwprintw(promptWindow, 0, (playSpace.width - continueTxt.length())/2, continueTxt.c_str());
    wrefresh(promptWindow);
}

void Board::clearPromptWin(){
    wclear(promptWindow);
    wrefresh(promptWindow);
}

void Board::roundOverPrompt(){
    wclear(promptWindow);
    const std::string roundOverTxt = "Round Over!";

    mvwprintw(promptWindow, 0, (playSpace.width - roundOverTxt.length())/2, roundOverTxt.c_str());
    wrefresh(promptWindow);
}

void Board::displayPlayerScore(){
    const std::string scoreTxt = "Points: %d";
    mvwprintw(handWindow, 1, 1, scoreTxt.c_str(), mainPlayer.getPoints());
    wrefresh(handWindow);
}

void Board::revealOpponentsHands(){
        for(Opponent opponent : opponents)
        for(int i = 0; i < (int)opponent.cardPositions.size(); ++i) 
            if(i < (int)opponent.handCards.size())
                mvwprintw(playWindow, opponent.cardPositions.at(i).y, opponent.cardPositions.at(i).x, opponent.handCards.at(i).sym);
            else
                mvwaddch(playWindow, opponent.cardPositions.at(i).y, opponent.cardPositions.at(i).x, ' ');

    wrefresh(playWindow);
}

void Board::clearBoard(){
    wclear(playWindow);
    wclear(handWindow);
    wclear(promptWindow);
}

void Board::resetBoard(){

    wclear(promptWindow);
    resetDeckAndDiscard();
    deal(3);
    drawPlayers();
    drawBoard();
    drawHand();
    drawOpponentHands();

    wrefresh(playWindow);
    wrefresh(handWindow);
    wrefresh(promptWindow);
}

void Board::resetDeckAndDiscard(){
    discard.clear();
    deck.clear();
    initDeck();
    shuffle();
}