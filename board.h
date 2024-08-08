#include <vector>
#include <ncursesw/curses.h>
#include <coord.h>
#include <player.h>
#include <card.h>

class Board{
    private:
        int numPlayers;
        std::vector<Card> deck;
        std::vector<Card> discard;
        std::vector<Player> players;
        WINDOW *play;
        WINDOW *hand;
        Coord playSelectorPosition;
        Coord handSelectorPosition;

        friend class Player;

    public:
        Board(int);
        ~Board();
        void movePlaySelector(Coord);
        void moveHandSelector(Coord);
        void drawHand();
        void drawBoard();
        void deal();
        void shuffle();
        void drawOpponentHands();
        void drawPlayers();
    private:
        void initDeck();
};