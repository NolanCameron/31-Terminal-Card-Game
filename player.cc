#include "player.h"

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

void Player::discardCard(Board& board, int cardIndex){
    board.discard.push_back(handCards.at(cardIndex));
    handCards.erase(handCards.begin()+cardIndex);
}

float Player::getScore(){
    return score;
}

float Player::calculateScore(){
    int suiteScore [4];
    for(int i = 0; i < 4; ++i) for(int j = 0; j < handCards.size(); ++j){
        if(suite(i) == handCards.at(j).s){
            suiteScore[i] += handCards.at(j).val;
        }
    }

    for(int i = 0; i < (int)handCards.size() - 1; i++)if(handCards[i].f != handCards[i + 1].f){
        return *std::max_element(suiteScore,suiteScore+4);
    }

    return 30.5;
}

void Opponent::chooseDraw(Board& board){

    int score = calculateScore();
    handCards.push_back(board.discard.back());
    if(score < calculateScore()){
        board.discard.pop_back();
    }else{
        handCards.pop_back();
        handCards.push_back(board.deck.back());
        board.deck.pop_back();
    }

}

float Opponent::calculateHand(std::vector<Card> hand){

    int suiteScore [4];
    for(int i = 0; i < 4; ++i) for(int j = 0; j < (int)handCards.size(); ++j){
        if(suite(i) == handCards.at(j).s){
            suiteScore[i] += handCards.at(j).val;
        }
    }

    for(int i = 0; i < (int)hand.size() - 1; i++)if(hand[i].f != hand[i + 1].f){
        return *std::max_element(suiteScore,suiteScore+4);
    }

    return 30.5;

}

void Opponent::chooseDiscard(Board& board){
    float val, max = 0;
    int index = 0;
    std::vector<Card> possibleHand (handCards.begin() + 1, handCards.end());
    for(int i = 0; i < (int)handCards.size(); ++i){
        val = calculateHand(possibleHand);
        if(val > max){
            index = i;
            max = val;
        }
        possibleHand.erase(possibleHand.begin());
        possibleHand.push_back(handCards.at(i));
    }

    handCards.erase(handCards.begin() + index);

}

void Opponent::makeMove(int turn, Board& board){

}