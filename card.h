#pragma once
#ifndef CARD_HH
#define CARD_HH

enum face{A, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
enum suite{DIAMONDS, SPADES, CLUBS, HEARTS};
inline constexpr char* symbols[]  = {"🃁","🃂","🃃","🃄","🃅","🃆","🃇","🃈","🃉","🃊","🃋","🃍","🃎","🂡","🂢","🂣","🂤","🂥","🂦","🂧","🂨","🂩","🂪","🂫","🂭","🂮","🃑","🃒","🃓","🃔","🃕","🃖","🃗","🃘","🃙","🃚","🃛","🃝","🃞","🂱","🂲","🂳","🂴","🂵","🂶","🂷","🂸","🂹","🂺","🂻","🂽","🂾"};
inline constexpr char* cardBack  = "🂠";

struct Card{
    suite s;
    face f;
    const char* sym;
    int val;
};

#endif