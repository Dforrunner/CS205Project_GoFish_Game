#include "Deck.h"
using namespace std;

Deck::Deck(){
    Suit suit[] = {spades, diamonds, clubs, hearts};
    for (int j=1; j<14; j++) {
        for (int i=0; i<4; i++) {
            deck.emplace_back(Card(j,suit[i]));
        }
    }

}

int Deck::getNumOfCardsDealt() const {
    return numOfCardsDealt;
}

void Deck::setNumOfCardsDealt(int numOfCardsDealt) {
    Deck::numOfCardsDealt = numOfCardsDealt;
}

void Deck::printDeck() {
    for(int i = 0; i < deck.size(); i++){
        cout << i+1 << ". " << deck[i] << endl;
    };
}
