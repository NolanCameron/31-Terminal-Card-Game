#include <locale.h>
#include <iostream>
#include <vector>
#include <ncursesw/curses.h>
#include <random>
#include <algorithm>
using namespace std;

enum face{A,TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
enum suite{DIAMONDS, SPADES, CLUBS, HEARTS};
const char* sym[] = {"🃁","🃂","🃃","🃄","🃅","🃆","🃇","🃈","🃉","🃊","🃋","🃍","🃎","🂡","🂢","🂣","🂤","🂥","🂦","🂧","🂨","🂩","🂪","🂫","🂭","🂮","🃑","🃒","🃓","🃔","🃕","🃖","🃗","🃘","🃙","🃚","🃛","🃝","🃞","🂱","🂲","🂳","🂴","🂵","🂶","🂷","🂸","🂹","🂺","🂻","🂽","🂾"};
const char* cardBack = "🂠";
const char* selector = "⮝";

struct coord{
    int y,x;
    coord(int j = 0, int i = 0){
        y = j;
        x = i;
    }
};

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

struct{
    coord card1(handSpace.height/2,(handSpace.width-6)/2);
    coord card2(handSpace.height/2,(handSpace.width-6)/2 + 2);
    coord card3(handSpace.height/2,(handSpace.width-6)/2 + 4);
    coord card4(handSpace.height/2,(handSpace.width-6)/2 + 6);
}cardSpace;

coord deckPostion(playSpace.height/2,playSpace.width/2-1);
coord discardPostion(playSpace.height/2,playSpace.width/2+1);

struct{
    coord deck(deckPostion.y + 1,deckPostion.x);
    coord discard(discardPostion.y + 1,discardPostion.x);
    coord card1(handSpace.card1.y + 1,handSpace.card1.x);
    coord card2(handSpace.card1.y + 1,handSpace.card1.x);
    coord card3(handSpace.card1.y + 1,handSpace.card1.x);
    coord card4(handSpace.card1.y + 1,handSpace.card1.x);
}selectorPosition;

struct Card{
    suite s;
    face f;
    const char* sym;
    int val;
};

class Player{

    public:

        vector<Card> handCards;
        struct{
            int x;
            int y;
        }handarea;
        
    Player(int y,int x){handarea.y = y; handarea.x = x;}

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

        initDeck();
        
        Player mainPlayer(handSpace.height/2-1,(handSpace.width-6)/2);
        players.push_back(mainPlayer);
        for(int i =1; i < numPlayers; ++i){players.push_back(Player(2,playSpace.width/(players.size()*2-1)*(i*2-1)));}

        WINDOW *play = newwin(playSpace.height, playSpace.width, playSpace.y,playSpace.x);
        WINDOW *hand = newwin(handSpace.height, handSpace.width, handSpace.y,handSpace.x);

        refresh();
        box(play,0,0);
        box(hand,0,0);
        wprintw(play,"31");
        wprintw(hand,"Hand");

        Shuffle();
        Deal(3,players,deck);
        drawPlayers(play);
        drawBoard(play);
        drawHand(hand);
        drawOpponentHands(play);

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

    void initDeck(){
        for(int i =0; i < 4; ++i) for(int j = 0; j < 13; ++j){
            deck.push_back({suite(i),face(j),sym[i*13+j],j>10 ? 10 : j});
        }
    }

    void drawPlayers(WINDOW *play){

        for(int i = 1; i < players.size(); ++i){
            mvwprintw(play,1,playSpace.width/(players.size()*2-1)*(i*2-1),"Player %d",i);
        } 

    }

    void drawOpponentHands(WINDOW *play){
        for(int i = 1; i < players.size(); ++i){
            for(int j =0; j < players.at(i).handCards.size(); ++j){
                mvwprintw(play, players.at(i).handarea.y, players.at(i).handarea.x+1+j*2,cardBack);
            }
        } 
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

        if(deck.size() > 0) mvwprintw(play,deckPostion.y,deckPostion.x,cardBack);
        mvwprintw(play,discardPostion.y,discardPostion.x,inPlay.sym);

    }

    void drawHand(WINDOW *hand){

        for(int i = 0; i < players.at(0).handCards.size(); ++i) mvwprintw(hand,handSpace.height/2-1,(handSpace.width-6)/2 +i*2,players.at(0).handCards.at(i).sym);

    }

    void moveSelector(coord Position){

    }

};


int main(int argc, char ** argv){

    setlocale(LC_ALL, "");
    initscr();
    curs_set(0);

    Board board(5);
    
    getch();

    return 0;
}
