#include <iostream>

#include "Card.h"
#include "Deck.h"
#include "Game.h"
#include <stdlib.h>
using namespace std;

// Returns true if s is a number else return false meaning it's a string
bool isNumber(string s) {
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;
    return true;
}

//contributors: JB, Muhammet, Liam
int main() {

    /* =================================== <GAME> ===================================== */

    //Setting up the game
    //Determine lie percent
    int lies = 10;
    Game smartGame = Game(lies);  //create new game

    //Prompt to Choose Mode - While (not quit)
    string modeChoice;
    cout << " --- Welcome to Go Fish ---" << endl;
    while (modeChoice != "0") {
        string playerUserName;
        cout << "Enter a username: " ;
        cin >> playerUserName;
        smartGame.setPlayerName(playerUserName);
        cout << endl;

        cout << "Please choose one of the following options:" << endl;
        cout << "0 - Quit" << endl;
        cout << "1 - Smart Mode (computer can remember your requests and will use them against you)" << endl;
        cout << "2 - Dumb Dumb Mode (computer cannot remember your requests)" << endl;
        cout << ">>> ";
        std::cin >> modeChoice;

        // ------------------- Start Game------------------- //
        string userResponse;
        int newGame;
        if(modeChoice == "1"){
            cout << "Welcome to smart mode.  It is your turn... "<< endl;
        }
        else{
            cout << "Welcome to dumb mode. It is your turn... "<<endl;
        }
        while(userResponse != "quit") {

            // ------------------- PLAYER TURN ------------------- //
            //Draw card if not enough cards
            while (true) {
                //If the user can still play
                if(smartGame.getDeckSize() == 0 && smartGame.userHand.empty()) {
                    break;
                }

                if (!smartGame.userHand.empty()) {
                    smartGame.printHand(1);  //Print user hand
                    //smartGame.printHand(2);  //Computer hand - should only be displayed when testing
                }

                //Check for books in case there is a book when they are dealt cards if there is a book print hand
                smartGame.checkBookAndPrint();

                if(smartGame.userHand.empty() && smartGame.computerHand.empty() && smartGame.getDeckSize() == 0){
                    modeChoice = "0";
                }

                cout << "For Help, enter '?' --- To Quit, enter 'quit'" << endl;
                cout << playerUserName << " make a guess >>> ";
                cin >> userResponse;  //Capture user response

                //Infinite loop that will break only if the userResponse is an integer.
                while (true) {

                    //Break loop if the user response is an number
                    if (isNumber(userResponse)) {
                        break;
                    }

                    //Catch exception if the user response is not an integer
                    try {
                        int x = stoi(userResponse);
                    } catch (invalid_argument &) {
                        while ((userResponse != "?") and (userResponse != "quit") and (userResponse != "b")) {
                            cout << "Invalid Input! Please enter the '?' for help || 'quit' to end game || 'b' to go back to game" << endl;
                            cout << ">>> ";
                            cin >> userResponse;
                        }
                        if (userResponse == "quit") {
                            exit(0);
                        } else if (userResponse == "?") {
                            cout << "print help" << endl;
                            smartGame.printHand(1);  //Print user hand
                        } else if (userResponse == "b") {
                            smartGame.printHand(1);  //Print user hand
                        }
                    }
                }
                //Convert user string input into an integer
                int userIntResponse = stoi(userResponse);

                //Checking to make sure we asked for a card in the hand!
                if(!smartGame.userHand.empty()) {
                    while (userIntResponse > smartGame.userHand.size() || userIntResponse < 0) {
                        cout << "Input not accepted. Please enter the integer value that is below one of the cards" << endl;
                        cout << "Guess again >>> " << endl;
                        cin >> userIntResponse;
                    }
                }

                //If user asks for a card in their hand
                if (userIntResponse > 0 && userIntResponse <= smartGame.userHand.size()) {
                    //Checks to see if computer has card and gives more turns if user gets it right
                    if (smartGame.askComputer(userIntResponse)) {

                        smartGame.takeCards(userIntResponse, 2); //Take card from computer(2)
                        cout << "\nYou got some cards!  Here's Another turn" << endl;
                        //Main purpose of this if statement is to only output the table header to a file in the first round
                        //then just add rows than after
                        if(newGame == 0) {
                            //File output
                            smartGame.fileIO(smartGame.userHand[userIntResponse-1], playerUserName, "Match Found",true);
                            newGame++;
                        }else{
                            //File output
                            smartGame.fileIO(smartGame.userHand[userIntResponse-1], playerUserName, "Match Found",false);
                        }
                    } else {
                        cout << "\nComputer: 'Go Fish! You must draw a card.'" << endl;
                        cout<<endl;

                        if(smartGame.getDeckSize() != 0) {
                            cout << playerUserName << ": You Drew " << endl;
                            smartGame.drawCard(1, (int) smartGame.userHand.size(), 1);
                        }else{
                            cout << "NO CARDS LEFT!" << endl;
                        }
                        cout << endl;

                        if(newGame == 0) {
                            //File output
                            smartGame.fileIO(smartGame.userHand[userIntResponse-1], playerUserName, "Match Not Found",true);
                            newGame++;
                        }else{
                            //File output
                            smartGame.fileIO(smartGame.userHand[userIntResponse-1], playerUserName, "Match Not Found",false);
                        }
                        //turn ended break loop
                        break;
                    }

                }

            }

            // ------------------- COMPUTER TURN ------------------- //
            //Checking to see if the game is still playable.
            if(smartGame.userHand.empty() && smartGame.computerHand.empty() && smartGame.getDeckSize() == 0){
                modeChoice = "0";
            }

            //Checking to see if the computer can still play
            if(smartGame.getDeckSize() == 0 && smartGame.userHand.empty()) {
                break;
            }

            bool turnEndingTrigger = true;
            while (turnEndingTrigger) {
                // Use Smart mode (memory)
                if(modeChoice == "1") {
                    turnEndingTrigger = smartGame.askUserSmart();
                // User dumb mode (no memory)
                }else {
                    turnEndingTrigger = smartGame.askUserDumb();
                }
            }
            //making sure that we've recorded every book
            smartGame.checkBookAndPrint();

            if(smartGame.userHand.empty() && smartGame.computerHand.empty() && smartGame.getDeckSize() == 0){
                modeChoice = "0";
            }
        }

    }
    if(smartGame.getUserScore() + smartGame.getComputerScore() == 13){
        if(smartGame.getUserScore() > smartGame.getComputerScore()){
            cout << "YOU ARE A CHAMPION, AND SHALL NEVER GO FISH AGAIN" << endl;
        }
        else{
            cout << "YOU HAVE LOST AND WILL FISH FOREVER" << endl;
        }
    }
    return 0;
}