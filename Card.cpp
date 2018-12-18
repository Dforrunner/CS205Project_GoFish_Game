#include "Card.h"

Card::Card() = default;

Card::Card(int r, Suit s) {
    rank = r;
    suit = s;
    name = to_string(r)+to_string(s);
}

int Card::getRank() const {
    return rank;
}

string Card::getRankString() const {
    if(rank==1){
        return "A";
    }
    if(rank==11){
        return "J";
    }
    if(rank==12){
        return "Q";
    }
    if(rank==13){
        return "K";
    }
    return to_string(rank);
}

Suit Card::getSuit() const{
    return suit;
}

string Card::getSuitString() const {
    switch(suit){
        case spades:
            return "♠";
        case diamonds:
            return "♦";
        case clubs:
            return "♣";
        case hearts:
            return "♥";
    }
}


string Card::getName() const {
    return name;
}

ostream& operator << (ostream& outs, const Card &c) {
    outs << c.rank << c.suit ;
    return outs;
}