#include <locale.h>
#include <iostream>
#include <vector>
#include <ncursesw/ncurses.h>
#include <random>
#include <algorithm>
using namespace std;

enum face{A,TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
enum suite{DIAMONDS, SPADES, CLUBS, HEARTS};
const char* sym[] = {"🃁","🃂","🃃","🃄","🃅","🃆","🃇","🃈","🃉","🃊","🃋","🃍","🃎","🂡","🂢","🂣","🂤","🂥","🂦","🂧","🂨","🂩","🂪","🂫","🂭","🂮","🃑","🃒","🃓","🃔","🃕","🃖","🃗","🃘","🃙","🃚","🃛","🃝","🃞","🂱","🂲","🂳","🂴","🂵","🂶","🂷","🂸","🂹","🂺","🂻","🂽","🂾"};
const char* cardBack = "🂠";

struct{
    int height = 14;
    int width = 68;
    int x = 1;
    int y = 0;
}playSpace;

struct{
    int height = playSpace.height - 10;
    int width = playSpace.width - 34;
    int x = playSpace.x + 17;
    int y = playSpace.y + 14;
}handSpace;

struct Card{
    suite s;
    face f;
    const char* sym;
    int val;
};

class Player{

    public:
        vector<Card> handCards;

    Player(){}

    void Draw(Card card){

        handCards.push_back(card);

    }

};

class Board{

    public:

        vector<Card> deck;
        vector<Card> discard;
        vector<Player> players;
        Card inPlay;
        WINDOW *play;
        WINDOW *hand;

    Board(int numPlayers){

        for(int i =0; i < 4; ++i) for(int j = 0; j < 13; ++j){
            deck.push_back({suite(i),face(j),sym[i*13+j],j>10 ? 10 : j});
        }
        
        players.resize(numPlayers);

        WINDOW *play = newwin(playSpace.height, playSpace.width, playSpace.y,playSpace.x);
        WINDOW *hand = newwin(handSpace.height, handSpace.width, handSpace.y,handSpace.x);

        refresh();
        box(play,0,0);
        box(hand,0,0);
        wprintw(play,"31");
        wprintw(hand,"Hand");

        Shuffle();
        Deal(3,players,deck);
        drawBoard(play);
        drawHand(hand);

        wrefresh(play);
        wrefresh(hand);

        wgetch(hand);
        wgetch(play);

    }

    ~Board(){

        delwin(play);
        delwin(hand);
        endwin();

    }

    void Shuffle(){

        auto rd = std::random_device {}; 
        auto rng = std::default_random_engine { rd() };
        std::shuffle(std::begin(deck), std::end(deck), rng);
        
    }

    void Deal(int amountCards,vector<Player>& players,vector<Card>& deck){

        for(int j = 0; j < amountCards; ++j) for(int i = 0; i < players.size(); ++i){
            players.at(i).Draw(deck.back());
            deck.pop_back();
        }

        inPlay = deck.back();
        deck.pop_back();

    }

    void drawBoard(WINDOW *play){

        if(deck.size() > 0) mvwprintw(play,playSpace.height/2,playSpace.width/2-1,cardBack);
        mvwprintw(play,playSpace.height/2,playSpace.width/2+1,inPlay.sym);

        //for(int i = 1; players.size() > i; ++i) 


    }

    void drawHand(WINDOW *hand){

        for(int i = 0; i < players.at(0).handCards.size(); ++i) mvwprintw(hand,handSpace.height/2-1,(handSpace.width-6)/2 +i*2,players.at(0).handCards.at(i).sym);

    }

};


int main(int argc, char ** argv){

    setlocale(LC_ALL, "");
    initscr();

    Board board(4);
    
    getch();

    return 0;
}
