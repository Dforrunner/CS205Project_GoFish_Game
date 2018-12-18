#include "Game.h"
using namespace std;

//CONSTRUCTOR
Game::Game() {
    userScore = 0;
    computerScore = 0;
    deck = Deck();
    shuffleDeck();
    dealCards(7);
}

Game::Game(int lies){
    userScore = 0;
    computerScore = 0;
    liePercent = lies;
    deck = Deck();
    shuffleDeck();
    dealCards(7);
}

//GETTERS
int Game::getUserScore() const {
    return userScore;
}
int Game::getComputerScore() const {
    return computerScore;
}
int Game::getDeckSize() const{
    return (int) deck.deck.size();
}
vector<Card> Game::getUserHand() const {
    return userHand;
}
vector<Card> Game::getComputerHand() const {
    return computerHand;
}
vector<Card> Game::getUserBooks() const {
    return userBooks;
}
vector<Card> Game::getComputerBooks() const {
    return computerBooks;
}
string Game::getPlayerName() const {
    return playerName;
}

//SETTERS
void Game::setUserScore(int u) {
    Game::userScore = u;
}
void Game::setComputerScore(int c) {
    Game::computerScore = c;
}
void Game::setUserHand(vector<Card> &h) {
    Game::userHand = h;
}
void Game::setComputerHand(vector<Card> &h) {
    Game::computerHand = h;
}
void Game::setComputerBooks(vector<Card> &computerBooks) {
    Game::computerBooks = computerBooks;
}
void Game::setUserBooks(vector<Card> &userBooks) {
    Game::userBooks = userBooks;
}
void Game::setPlayerName(string playerName) {
    Game::playerName = playerName;
}

void Game::shuffleDeck(){
    random_device rd;
    mt19937 g(rd());
    shuffle(deck.deck.begin(), deck.deck.end(), g);
}
//OTHER METHODS
void Game::dealCards(int numOfCards){
    shuffleDeck();
    //iterator
    auto it = next(deck.deck.begin(), numOfCards);

    //moves cards to playerHand vector
    move(deck.deck.begin(), it, back_inserter(userHand));
    //After it's been moved it erases those slots from the deck
    deck.deck.erase(deck.deck.begin(), it);

    //same things below for computerHand
    move(deck.deck.begin(), it, back_inserter(computerHand));
    deck.deck.erase(deck.deck.begin(), it);

}

void Game::drawCard(int playerNum, int handSize, int printOrNot){
    if(deck.deck.size() == 0) return;
    auto it = next(deck.deck.begin(), 1); //initially draw 1 card
    int numCardsToDraw = 1;

    if(playerNum == 1 and getDeckSize() > 0){
        move(deck.deck.begin(), it, back_inserter(userHand));
        deck.deck.erase(deck.deck.begin(), it);
    }else if (playerNum == 2 and getDeckSize() > 0){
        move(deck.deck.begin(), it, back_inserter(computerHand));
        deck.deck.erase(deck.deck.begin(), it);
    }else{
        cout << "CARD NOT DRAWN! For drawCard please specify 1 or 2 (1 = user player. 2 = computer player)" << endl;
    }

    //PRINT CARDS THAT HAVE BEEN DRAWN
    if(printOrNot == 1) {
        printDrawnCards(playerNum, numCardsToDraw);
    };

}


bool Game::askComputer(int response){
    bool returnValue = false;
    int rank = userHand[response-1].getRank();
    //computer now "knows" user has this card
    recordToMemory(rank);
    for(int i = 0; i < computerHand.size(); i++){
        if(rank == computerHand[i].getRank()){
            returnValue = true;
        }
    }
    if(returnValue){
        //seeding rand
        srand(time(NULL));
        //determine if the computer will lie
        int lieRoll = rand() % 100;
        //if the roll is below the preset lie percentage, act like
        if(lieRoll < liePercent){
            returnValue = false;
            //for testing purposes
            //cout << "I'm lying" << endl;
        }
        //else{
            //for testing
            //cout << "I'm telling the truth" << endl;
        //}

    }
    return returnValue;
}
bool Game::askUserSmart(){
    bool returnValue = false;
    //Get rank from memory to ask for
    int askSmartRank = compareHandToMemory();
    //If askSmart = 0, there is no match in memory, so we can ask dumb
    if(askSmartRank == 0){
        returnValue = askUserDumb();
    } else {
        for (int i = 0; i < userHand.size(); i++) {
            //If the rank matches card in user hand
            if (askSmartRank == userHand[i].getRank()) {
                returnValue = true;
                //take the card from player
                int cardPositionInHand = 0;
                //Loop through computers hand to find position in hand that the card has rank
                for(int j = 0; j < computerHand.size(); j++){
                    if(askSmartRank == computerHand[j].getRank()){
                        cardPositionInHand = j;
                    }
                }
                takeCards(cardPositionInHand, 1);  //take Card matching
                //Erase this rank from memory
                deleteFromMemory(askSmartRank);
            }
        }
    }
    return returnValue;
}

bool Game::askUserDumb(){
    bool returnValue = false;
    //seeding rand
    srand(time(NULL));
    //Generates a rand int between 0 and computerHand size
    int randGuess;
    try{
        randGuess = rand() % (int) computerHand.size();
    }catch(exception e){
        return false;
    }

    if( randGuess == 0 and computerHand.size() == 0){
        return false;
    }
    cout << "Computer: Asks for" << endl;
    printComputerGuessCard(randGuess); //print the guessed card (for real player to see

    for(int i = 0; i < userHand.size(); i++){
        if(computerHand[randGuess].getRank() == userHand[i].getRank()){
            takeCards(randGuess,1);
            returnValue = true;
            break;
        }
    }

    if(!returnValue){
        cout << getPlayerName() << ": Don't have any. Go fish!" << endl;
        cout << "Computer: Drew " << endl;
        drawCard(2, (int) computerHand.size(),1);
        computerFileIO(2, randGuess); //output computer guess as no match
    }
    else{
        cout << getPlayerName() <<": Nice guess! Here you go." << endl;
        cout << "Computer: Thanks for the card!" << endl;
        computerFileIO(1, randGuess); //output computer guess as match found
    }
    return returnValue;
}

void Game::takeCards(int card, int playerNum){
    if(playerNum == 2){
        //Player takes from computer
        for(int i = 0; i < computerHand.size(); i++){
            //userHan == computer
            if(computerHand[i].getRank() == userHand[card-1].getRank()){
                userHand.push_back(computerHand[i]);
                computerHand.erase(computerHand.begin()+i);
                i = 0;
            }
        }
    }else{
        //Computer takes from player
        for(int i = 0; i < userHand.size(); i++){
            if(userHand[i].getRank() == computerHand[card].getRank()){
                computerHand.push_back(userHand[i]);
                userHand.erase(userHand.begin()+i);
                //erase this rank from memory as user no longer has it
                deleteFromMemory(card);
                i = 0;
            }
        }
    }
}
/* Here is requirements for this method:
 *  - It needs to check to see if there is 4 matching ranks(aka cards)
 *  - If there is then it should add them to separate vector and remove them from the hand so that it can't be used for the rest of the game
 *  - We'll use the vector of books each player has to determine the winner. The player with the most books in their vector will be the winner.
 */
bool Game::checkForBook(int playerNum){
    bool bookFound = false;

    //Create an array to hold matching rank count
    int rankCount[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //Holds 13 ranks

    if(playerNum == 1) {
        //Loop through each card in hand and add matching ranks

        for (int i = 1; i < 14; i++) {
            int count = 0;
            for (int j = 0; j < userHand.size(); j++) {
                //Add one to rankCount@ current card rank (-1 because of arrays)
                if (i == userHand[j].getRank()) {
                    count++;
                }
            }
            rankCount[i - 1] += count;
        }

        //Loop through rankCount to see if any have 4 --- i+1 = rank
        for (int i = 0; i < 13; i++) {
            //If a book is in the hand (4 of a kind), loop through hand and remove those cards
            if (rankCount[i] == 4) {
                userScore++;
                deleteFromMemory(i);
                int cardsRemoved = 0;

                for (int j = 0; j < userHand.size(); j++) {
                    //If the rank matches, remove the card
                    if (userHand[j].getRank() == i + 1) {
                        userHand.erase(userHand.begin() + j);
                        cardsRemoved++;
                        j = -1;
                    }
                }

                bookFound = true;
                deleteFromMemory(rankCount[i]);  //Delete from memory
            }
        }


    }else{  //Code blow is same as the one above, but for the computer hand.
        for (int i = 1; i < 14; i++) {
            int count = 0;
            for (int j = 0; j < computerHand.size(); j++) {
                //Add one to rankCount@ current card rank (-1 because of arrays)
                if (i == computerHand[j].getRank()) {
                    count++;
                }
            }
            rankCount[i - 1] += count;
        }

        //Loop through rankCount to see if any have 4 --- i+1 = rank
        for (int i = 0; i < 13; i++) {
            //If a book is in the hand (4 of a kind), loop through hand and remove those cards
            if (rankCount[i] == 4) {
                computerScore++;
                deleteFromMemory(i);
                int cardsRemoved = 0;

                for (int j = 0; j < computerHand.size(); j++) {
                    //If the rank matches, remove the card
                    if (computerHand[j].getRank() == i + 1) {
                        computerHand.erase(computerHand.begin() + j);
                        cardsRemoved++;
                        j = -1;
                    }
                }

                bookFound = true;
                deleteFromMemory(rankCount[i]);  //Delete from memory
            }
        }
    }

    return bookFound;
}

void Game::checkBookAndPrint(){
    //Check for books in case there is a book when they are dealt cards if there is a book print hand
    bool playerHasBook = false;
    if(!userHand.empty()) {
        playerHasBook = checkForBook(1);
        if (playerHasBook) {
            cout << "----------------------------------------------------------------------" << endl;
            cout << "Books found in " << getPlayerName() << "'s hand. Hand after book has been taken out: " << endl;
            cout << "----------------------------------------------------------------------" << endl;
            drawCard(1, (int) userHand.size(), 0);
            printHand(1);  //Print user hand
        }
    }

    bool computerHasBook = false;
    if(!computerHand.empty()) {
        computerHasBook = checkForBook(2);
        if (computerHasBook) {
            cout << "----------------------------------------------------------------------" << endl;
            cout << "Books found in Computer hand. Hand after book has been taken out: " << endl;
            cout << "----------------------------------------------------------------------" << endl;
            drawCard(2, (int) computerHand.size(), 0);
            //printHand(2);  //Computer hand - should only be displayed when testing
        }
    }

    if (playerHasBook || computerHasBook){
        cout << "SCORES: " << endl;
        cout << "User: " + to_string(getUserScore()) + "   Computer: " +  to_string(getComputerScore()) << endl;
        cout << "----------------------------------------------------------------------" << endl;
    }
}

//========================================== FILE IO ==========================================//
void Game::fileIO(Card chosenCard, string playerUserName,string matchStatus, bool newGame){
    ofstream f("gameRecords.txt", ios_base::app);
    if (f.is_open()) {

        if(newGame) {
            f << "\n\n================================== NEW GAME ==================================" << endl;
            f << "Player Username: " << playerUserName << endl;
            f << "------------------------------------------------------------------------------" << endl;
            f << setw(17) << "Player Turn" << setw(8) << "|" << setw(20) << "Guess/Choice" << setw(10) << "|" << setw(20) << "Match Status" << endl;
            f << "------------------------------------------------------------------------------" << endl;
        };
        if (chosenCard.getRank() == 1 or chosenCard.getRank() > 10) {
            f << setw(13) << playerUserName << setw(12) << "|" << setw(15) << chosenCard.getRankString()
              << chosenCard.getSuitString() << setw(15) << "|" << setw(19) << matchStatus << endl;
        }else{
            f << setw(13) << playerUserName << setw(12) << "|" << setw(15) << chosenCard.getRank()
              << chosenCard.getSuitString() << setw(15) << "|" << setw(19) << matchStatus << endl;
        };


        f.close();
    }
};

void Game::computerFileIO(int matchStatus, int computerGuess){
    if(matchStatus == 1) {
        //File output
        fileIO(computerHand[computerGuess], "Computer", "Match Found",false);
    }else{
        fileIO(computerHand[computerGuess], "Computer", "Match Not Found",false);
    }
}
//========================================== MEMORY STUFF ==========================================//
void Game::recordToMemory(int guess){
    memory.push_back(guess);  //Saves the users guess to memory
}

void Game::deleteFromMemory(int rank){
    for(int i = 0; i < memory.size(); i++){
        if(memory[i]==rank){
            memory.erase(memory.begin()+i);
        }
    }
}

int Game::compareHandToMemory(){
    int counter = 0;
    for(int i = 0; i < memory.size(); i++){
        for(int j = 0; j < computerHand.size(); j++){
            if(memory[i] == computerHand[j].getRank()){
                counter++;
                return memory[i];
            }
        }
    }
    if(counter == 0){
        return 0;
    }
}


//========================================== PRINT METHODS ==========================================//

void Game::printHand(int playerNum){
    if(playerNum == 1) {
        cout << "******************************** Your Hand  **********************************" << endl;
        for (int i = 0; i < userHand.size(); i++) {
            cout << " -----   ";
        };
        cout << endl;
        for (int i = 0; i < userHand.size(); i++) {
            cout << "|     |  ";
        };
        cout << endl;

        for (int i = 0; i < userHand.size(); i++) {
            if(userHand[i].getRank() != 10) {
                cout << "| "  << userHand[i].getRankString() << userHand[i].getSuitString()  << "  |  ";
            }else{
                cout << "| " << userHand[i].getRankString() << userHand[i].getSuitString() << " |  ";
            };
        };
        cout << endl;
        for (int i = 0; i < userHand.size(); i++) {
            cout << "|     |  ";
        };
        cout << endl;
        for (int i = 0; i < userHand.size(); i++) {
            cout << " -----   ";
        };
        cout << endl;
        for (int i = 0; i < userHand.size(); i++) {
            if((i+1) < 10) {
                cout << "   " << i + 1 << "     ";
            }else{
                cout << "   " << i + 1 << "    ";
            }
        };
        cout << endl;

    }else if (playerNum == 2) {
        cout << "******************************  Computer Hand  *******************************" << endl;
        for (int i = 0; i < computerHand.size(); i++) {
            cout << " -----   ";
        };
        cout << endl;
        for (int i = 0; i < computerHand.size(); i++) {
            cout << "|     |  ";
        };
        cout << endl;

        for (int i = 0; i < computerHand.size(); i++) {
            if(computerHand[i].getRank() != 10) {
                cout << "| "  << computerHand[i].getRankString() << computerHand[i].getSuitString()  << "  |  ";
            }else{
                cout << "| " << computerHand[i].getRankString() << computerHand[i].getSuitString() << " |  ";
            };
        };
        cout << endl;
        for (int i = 0; i < computerHand.size(); i++) {
            cout << "|     |  ";
        };
        cout << endl;
        for (int i = 0; i < computerHand.size(); i++) {
            cout << " -----   ";
        };
        cout << endl;
        for (int i = 0; i < computerHand.size(); i++) {
            if((i+1) < 10) {
                cout << "   " << i + 1 << "     ";
            }else{
                cout << "   " << i + 1 << "    ";
            }
        };
        cout << endl;

    }else{
        cout << "CAN'T DISPLAY PLAYER HAND! For printHand(int playerHand) please specify 1 or 2 (1 = user player. 2 = computer player)" << endl;
    }

}

void Game::printDrawnCards(int playerNum, int cardIndexs){
    if(playerNum == 1) {
        for (int i = ((int) userHand.size() - cardIndexs); i < userHand.size(); i++) {
            cout << " -----   ";
        };
        cout << endl;
        for (int i = ((int) userHand.size() - cardIndexs); i < userHand.size(); i++) {
            cout << "|     |  ";
        };
        cout << endl;

        for (int i = ((int) userHand.size() - cardIndexs); i < userHand.size(); i++) {
            if(userHand[i].getRank() != 10) {
                cout << "| "  << userHand[i].getRankString() << userHand[i].getSuitString()  << "  |  ";
            }else{
                cout << "| " << userHand[i].getRankString() << userHand[i].getSuitString() << " |  ";
            };
        };
        cout << endl;
        for (int i = ((int) userHand.size() - cardIndexs); i < userHand.size(); i++) {
            cout << "|     |  ";
        };
        cout << endl;
        for (int i = ((int) userHand.size() - cardIndexs); i < userHand.size(); i++) {
            cout << " -----   ";
        };
        cout << endl;

    }else if (playerNum == 2) {
        for (int i = ((int) computerHand.size() - cardIndexs); i < computerHand.size(); i++)  {
            cout << " -----   ";
        };
        cout << endl;
        for (int i = ((int) computerHand.size() - cardIndexs); i < computerHand.size(); i++) {
            cout << "|     |  ";
        };
        cout << endl;

        for (int i = ((int) computerHand.size() - cardIndexs); i < computerHand.size(); i++) {
            if(computerHand[i].getRank() != 10) {
                cout << "| "  << computerHand[i].getRankString() << computerHand[i].getSuitString()  << "  |  ";
            }else{
                cout << "| " << computerHand[i].getRankString() << computerHand[i].getSuitString() << " |  ";
            };
        };
        cout << endl;
        for (int i = ((int) computerHand.size() - cardIndexs); i < computerHand.size(); i++) {
            cout << "|     |  ";
        };
        cout << endl;
        for (int i = ((int) computerHand.size() - cardIndexs); i < computerHand.size(); i++) {
            cout << " -----   ";
        };
        cout << endl;

    }else{
        cout << "CAN'T DISPLAY PLAYER HAND! For printHand(int playerHand) please specify 1 or 2 (1 = user player. 2 = computer player)" << endl;
    }
};

void Game::printComputerGuessCard(int cardIndex){
    cout << " -----   " << endl;
    cout << "|     |  " << endl;
    if(computerHand[cardIndex].getRank() != 10) {
        cout << "| "  << computerHand[cardIndex].getRankString() << computerHand[cardIndex].getSuitString()  << "  |  " << endl;;
    }else{
        cout << "| " << computerHand[cardIndex].getRankString() << computerHand[cardIndex].getSuitString() << " |  " << endl;
    };
    cout << "|     |  " << endl;
    cout << " -----   " << endl;

}
