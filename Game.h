#ifndef CS205PROJECT1_GAME_H
#define CS205PROJECT1_GAME_H

#include <vector>
#include <utility>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include "Deck.h"
using namespace std;

/*
 * represents the game of Go Fish.
 * contains all methods required to run a game.
 * should be instanced, and methods called on the game object.
 */
class Game {
private:
    int userScore = 0;
    int computerScore = 0;
    string playerName;
    int liePercent = 0;

public:
    Deck deck;
    vector<Card> userHand;
    vector<Card> computerHand;
    //these vectors store books of each player, for the purpose of scoring
    vector<Card> userBooks;
    vector<Card> computerBooks;
    /*
     * memory stores the ranks of all cards the user has asked for.
     * the computer will prioritize asking for these values if it can,
     * as it knows the user has them.
     */
    vector<int> memory;

    //Default constructors
    //contributors: JB
    Game();

    //constructor with lying
    //lies is an int from 0-100, inclusive, that determines the percent chance
    //that the computer will lie when asked for a card.
    //contributors: JB, Liam
    Game(int lies);

    //Getters
    int getUserScore() const;
    int getComputerScore() const;
    int getDeckSize() const;
    string getPlayerName() const;
    vector<Card> getUserHand() const;
    vector<Card> getComputerHand() const;
    vector<Card> getUserBooks() const;
    vector<Card> getComputerBooks() const;

    //Setters
    void setUserScore(int userScore);
    void setComputerScore(int computerScore);
    void setUserHand(vector<Card> &userHand);
    void setComputerHand(vector<Card> &computerHand);
    void setUserBooks(vector<Card> &userBooks);
    void setComputerBooks(vector<Card> &computerBooks);
    void setPlayerName(string playerName);

    //Shuffles the deck
    //contributors: Muhammet
    void shuffleDeck();

    /*
     * This function does the following: Shuffle deck,
     * then deal the number of cards specified to playerHand and computerHand,
     * then remove them from the deck.
     * contributors: Muhammet
     */
    void dealCards(int numOfCards);

    //Draws card from the top of the deck and removes it from the deck and adds it to the player or computer hand
    //contributors: JB, Muhammet, Liam
    void drawCard(int playerNum, int handSize, int printOrNot); // 1 = Real Player, 2 = computer player

    //Prints the hand. Made it print them out fancy
    //contributors: Liam, Muhammet
    void printHand(int playerNum);

    //Prints cards that have been drawn by players
    //contributors: Muhammet
    void printDrawnCards(int playerNum, int cardIndexs);

    //Prints a single card to display the card the computer guessed
    //contributors: Muhammet
    void printComputerGuessCard(int cardIndex);


    /*
     * asks the computer if it has a card matching the given position of the player's hand.
     * returns true if there is a match
     * returns false if there is no match or the computer decides to lie
     * contributors: JB, Muhammet, Liam
     */
    bool askComputer(int response);

    /*
     * asks the user for a card, prioritizing cards in memory.
     * if a match is found, takes card(s)
     * if memory is empty, call askUserDumb.
     * contributors: Liam, JB
     */
    bool askUserSmart();

    /*
     * asks the user for a random card in the computer's hand.
     * if a match is found, takes card(s)
     * contributors: JB, Muhammet
     */
    bool askUserDumb();

    /*
     * moves cards from one player's hand to the other
     * contributors: JB
     */
    void takeCards(int card, int playerNum);

    /*
     * checks the given player's hand for books (4 of a kind)
     * if it finds any, it removes them and adds to their score.
     * contributors: JB, Muhammet
     */
    bool checkForBook(int playerNum);

    void checkBookAndPrint();


    /* Save guesses and game activity to a file
     * contributors: Muhammet
     */
    void fileIO(Card chosenCard, string playerUserName,string matchStatus, bool newGame = true);
    void computerFileIO(int matchStatus, int computerGuess);

    /*===========================MEMORY MANIPULATION===========================*/
    //records the guess to memory
    //contributors: Liam
    void recordToMemory(int guess);

    //Deletes that rank from memory
    //contributors: Liam
    void deleteFromMemory(int rank);

    //Returns rank of card to ask for, or 0 if nothing in hand matches memory
    //contributors: Liam
    int compareHandToMemory();




};

#endif //CS205PROJECT1_GAME_H
