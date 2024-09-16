#include "player.hh"
#include <random>

Player::Player(int y, int x): 
    points(0),
    knockVal(false)
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
    handCards.erase(handCards.begin() + cardIndex);
}

int Player::getPoints(){
    return points;
}

float Player::calculateScore(){
    int suiteScore [4] = {1};
    for(int i = 0; i < 4; ++i) for(int j = 0; j < (int)handCards.size(); ++j){
        if(suite(i) == handCards.at(j).s){
            suiteScore[i] += handCards.at(j).val;
        }
    }

    for(int i = 0; i < (int)handCards.size() - 1; i++)if(handCards[i].f != handCards[i + 1].f){
        return *std::max_element(suiteScore,suiteScore+4);
    }

    return 30.5;
}

int Player::handSize(){
    return (int)handCards.size();
}

bool Player::knocked(){
    return knockVal;
}

void Player::knock(){
    knockVal = true;
}

bool Player::is31(){
    return calculateScore() > 30.5;
}

void Player::addPoints(int i){
    points += i;
}

void Player::clearHand(){
    handCards.clear();
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

    board.discard.push_back(handCards.at(index));
    handCards.erase(handCards.begin() + index);

}

void Opponent::makeMove(int turn, bool& knock, Board& board){
    if(chooseKnock(turn, knock)){
        knockVal = true;
        knock = true;
        return;
    }
    chooseDraw(board);
    board.drawOpponentHands();
    board.drawBoard();
    getch();
    chooseDiscard(board);
    board.drawOpponentHands();
    board.drawBoard();
    getch();
}

bool Opponent::chooseKnock(int turn, bool knock){

    std::random_device dev;
    std::mt19937 rng(dev());

    const float startHandVal = 14;//rough average starting hand val
    const float tolerance = 4;
    const float turnIncrement = 0.3;
    const float maxNonKnockVal = 30;
    const float randomnessAbsRange = 2;
    std::uniform_real_distribution<float> random(-randomnessAbsRange,randomnessAbsRange);

    float currentScore = calculateScore();

    if((startHandVal + tolerance + turnIncrement*turn + (float)random(rng) < currentScore || maxNonKnockVal < currentScore) && knock == false)
        return true;
    

    return false;

}

