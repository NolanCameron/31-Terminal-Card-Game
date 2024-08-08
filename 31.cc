#include <locale.h>
#include <iostream>
#include <vector>
#include <ncursesw/curses.h>
#include <random>
#include <algorithm>
#include <board.h>
#include <player.h>


enum face{A,TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
enum suite{DIAMONDS, SPADES, CLUBS, HEARTS};
const char* sym[] = {"🃁","🃂","🃃","🃄","🃅","🃆","🃇","🃈","🃉","🃊","🃋","🃍","🃎","🂡","🂢","🂣","🂤","🂥","🂦","🂧","🂨","🂩","🂪","🂫","🂭","🂮","🃑","🃒","🃓","🃔","🃕","🃖","🃗","🃘","🃙","🃚","🃛","🃝","🃞","🂱","🂲","🂳","🂴","🂵","🂶","🂷","🂸","🂹","🂺","🂻","🂽","🂾"};
const char* cardBack = "🂠";
const char* selector = "⮝";
enum positions{discard,deck,card1,card2,card3,card4};

struct Coord{
    int y,x;
    Coord(int j = 0, int i = 0): y(j), x(i){}
};

struct windowSpace{
    int height;
    int width;
    int x;
    int y;
    windowSpace(int h,int w,int j, int i): height(h), width(w), x(i), y(j){}
};

struct cardPositions{
    std::vector<Coord> cardCoords;
    cardPositions(Coord firstPosition){for(int i = 0; i < 4; ++i){cardCoords.push_back(Coord(firstPosition.y, firstPosition.x + i*2));}}
};

windowSpace playSpace(14,68,0,1);
windowSpace handSpace(playSpace.height - 10,playSpace.width - 34,playSpace.y + 14,playSpace.x + 17);

Coord deckPostion(playSpace.height/2,playSpace.width/2-1);
Coord discardPostion(playSpace.height/2,playSpace.width/2+1);

struct Card{
    suite s;
    face f;
    const char* sym;
    int val;
};

class Player{

    std::vector<Card> handCards;
    cardPositions handArea;
    int score;

    friend class Board;

    public:

        Player(int y, int x): handArea(Coord(y,x)), score(0){}

        void drawFromDeck(Board& board){
            handCards.push_back(board.deck.back());
            board.deck.pop_back();
        }

        void drawFromDiscard(Board& board){
            handCards.push_back(board.discard.back());
            board.discard.pop_back();
        }

        int getScore(){
            return score;
        }

        int calculateScore(){
            int suiteScore [4];
            for(int i = 0; i < 4; ++i) for(int j = 0; j < handCards.size(); ++j){
                if(suite(i) == handCards.at(j).suite){
                    suiteScore[i] += handCards.at(j).val;
                }
            }

            return std::max_element(suiteScore,suiteScore+4);

        }
};

class Board{

    int numPlayers;
    std::vector<Card> deck;
    std::vector<Card> discard;
    std::vector<Player> players;
    WINDOW *play;
    WINDOW *hand;
    Coord playSelectorPosition;
    Coord handSelectorPosition;

    friend class Player;

    void initDeck(){
        for(int i = 0; i < 4; ++i) for(int j = 0; j < 13; ++j){
            deck.push_back({suite(i),face(j),sym[i*13+j],j>10 ? 10 : j});
        }
    }

    void drawPlayers(){

        for(int i = 1; i < players.size(); ++i){
            mvwprintw(play,1,playSpace.width/(players.size()*2-1)*(i*2-1),"Player %d",i);
        } 

    }

    void drawOpponentHands(){
        for(int i = 1; i < players.size(); ++i){
            for(int j = 0; j < players.at(i).handCards.size(); ++j){
                mvwprintw(play, players.at(i).handArea.cardCoords.at(j).y, players.at(i).handArea.cardCoords.at(j).x, cardBack);
            }
        } 
    }

    void Shuffle(){

        auto rd = std::random_device {}; 
        auto rng = std::default_random_engine { rd() };
        std::shuffle(std::begin(deck), std::end(deck), rng);
        
    }

    void Deal(int amountCards){

        for(int j = 0; j < amountCards; ++j) for(int i = 0; i < players.size(); ++i){
            players.at(i).drawFromDeck(*this);
        }

        discard.push_back(deck.back());
        deck.pop_back();

    }

    void drawBoard(){

        if(deck.size() > 0) mvwprintw(play, deckPostion.y, deckPostion.x, cardBack);
        if(discard.size() != 0) {mvwprintw(play, discardPostion.y, discardPostion.x, discard.back());}
        else{mvwdelch(play, discardPostion.y, discardPostion.x);}

    }

    void drawHand(){

        for(int i = 0; i < players.at(0).handCards.size(); ++i) mvwprintw(hand, players.at(0).handArea.cardCoords.at(i).y, players.at(0).handArea.cardCoords.at(i).x, players.at(0).handCards.at(i).sym);

    }

    public:
        Board(int);
        ~Board();
        void movePlaySelector(Coord);
        void moveHandSelector(Coord);


};

Board::Board(int numPlayers): play(newwin(playSpace.height, playSpace.width, playSpace.y, playSpace.x)), hand(newwin(handSpace.height, handSpace.width, handSpace.y, handSpace.x)), numPlayers(numPlayers), handSelectorPosition(Coord(0,0)), playSelectorPosition(Coord(0,0)){

        initDeck();
        players.push_back(Player(handSpace.height/2-1,(handSpace.width-6)/2));
        for(int i = 1; i < numPlayers; ++i){players.push_back(Player(2,playSpace.width/(numPlayers*2-1)*(2*i-1)));}

        refresh();
        box(play,0,0);
        box(hand,0,0);
        wprintw(play,"31");
        wprintw(hand,"Hand");

        Shuffle();
        Deal(3);
        drawPlayers();
        drawBoard();
        drawHand();
        drawOpponentHands();

        wrefresh(play);
        wrefresh(hand);

        wgetch(hand);
        wgetch(play);

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

class Game{

};

int main(int argc, char ** argv){

    setlocale(LC_ALL, "");
    initscr();
    curs_set(0);

    Board board(5);
    
    getch();

    return 0;
}
