#ifndef CS205PROJECT1_CARD_H
#define CS205PROJECT1_CARD_H

#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <string>
using namespace std;

enum Suit{spades, diamonds, clubs, hearts};
/*
 * class that represents a standard playing card.
 * has an integer for rank (should be 1-13)
 * and a suit from an enum
 *
 * author: Liam
 */
class Card {
private:
    int rank;
    Suit suit;
    string name = to_string(rank)+to_string(suit);

public:
    //default constructors
    Card();
    Card(int r, Suit s);

    //Getters
    int getRank() const;
    //returns rank, replacing 1, 11, 12, 13 with A, J, Q, K
    string getRankString() const;
    Suit getSuit() const;
    //returns suit as its symbol for the purpose of printing card face
    string getSuitString() const;
    string getName() const;

    //Overloading operator
    friend ostream& operator << (ostream& outs, const Card &c);
};


#endif //CS205PROJECT1_CARD_H
