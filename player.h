#include <vector>

class Player{

    std::vector<Card> handCards;
    std::vector<Coord> cardPositions;
    int score;

    friend class Board;

    public:

        Player(int y, int x);
        void drawFromDeck(Board& board);
        void drawFromDiscard(Board& board);
        int getScore();
        int calculateScore();
};