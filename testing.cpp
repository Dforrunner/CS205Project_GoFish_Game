
#include <iostream>

#include "Card.h"
#include "Deck.h"
#include "Game.h"
#include <stdlib.h>
using namespace std;

int main() {
Game g = Game();

/**
 * The code below is mainly for testing the methods
 */
/*
g.deck.printDeck(); //print the deck
cout<< "-------------------" << endl;
g.deck.shuffleDeck(); //this will shuffle the deck, but it's not necessary to use outside of testing
cout<< "-------------------" << endl;
g.deck.printDeck(); //again mainly for testing purposes
cout<< "-------------------" << endl;
g.dealCards(7); //When this is called it will shuffle the cards. Deal the cards. And print them, but that can be changed when needed
*/
//    g.deck.printDeck();
//    h.drawCard(1); //this will draw a card. use the parameter 1 to draw card for human player and user 2 to draw card for computer
//    h.printHands(1); //1 = print hand for human player
//    h.printHands(2); // 2 = print hand for computer player
//    h.printDeck();    //this will print the deck after the cards have been dealt and removed from the deck.

/**
 * Working on the method below. The problem I am encountering is the vectors for playerHand and computerHand are empty
 * even after the cards have been dealt. Now it is populated when you print it in by calling h.pringHands(), but it's empty
 * when you use the printHands function in the checkForBook() function. I feel like I am missing a very simple point there,
 * so some help would be cool.
 */
//    g.printHand(1);
//    g.checkForBook(1);
//    cout<<endl;
    g.printHand(1);
    g.checkForBook(1);
    g.printHand(1);
return 0;
}