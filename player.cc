#include <player.h>
#include <coord.h>
#include <board.h>
#include <card.h>
#include <algorithm>

Player::Player(int y, int x): 
    score(0)
{
    for(int i = 0; i < 4; ++i)cardPositions.push_back(Coord(y, x + i*2));
}

void Player::drawFromDeck(Board& board){
    handCards.push_back(board.deck.back());
    board.deck.pop_back();
}

void Player::drawFromDiscard(Board& board){
    handCards.push_back(board.discard.back());
    board.discard.pop_back();
}

int Player::getScore(){
    return score;
}

int Player::calculateScore(){
    int suiteScore [4];
    for(int i = 0; i < 4; ++i) for(int j = 0; j < handCards.size(); ++j){
        if(suite(i) == handCards.at(j).s){
            suiteScore[i] += handCards.at(j).val;
        }
    }

    return *std::max_element(suiteScore,suiteScore+4);

}