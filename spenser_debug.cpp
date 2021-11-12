//ECE 309 Go Fish Project
//Team Vector
//Spenser Geideman, Maritza Aguirre-Diaz, Tyler Karpowich

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <random>
#include <ctime>
#include <stdlib.h>


/* TO DO
Slow CPU playthrough (Enter or time delay)
*/

using namespace std;

//define suits
#define HEARTS 'h'
#define SPADES 's'
#define DIAMONDS 'd'
#define CLUBS 'c'

class Deck{
protected:
    //our deck and players hands will be a linked list
    class cardNode{//card has a number and suit
    public:
        cardNode(int n, char s):number(n), suit(s), next(nullptr){}
        int number;
        char suit;
        cardNode* next;
    };
    cardNode *head;
    cardNode *tail;
public:
    //default constructor
    Deck(){
        head = nullptr;
        tail = nullptr;
    }
    //destructor
    ~Deck(){
        while(!empty()){
            //remove card from head only if not an empty list
            if (head == nullptr){//empty list
                //do nothing
            }else{//not empty list
                cardNode *tmp = head;
                head = head ->next;
                if(head == nullptr)//now an empty list
                    tail = nullptr;
                delete tmp;
            }
        }
    }
    //copy constructor
    Deck(const Deck& other){
        head = nullptr;
        tail = nullptr;
        cardNode * tmp = other.head;
        while(tmp){
            insertCard(tmp->number, tmp->suit);
            tmp = tmp->next;
        }
    }
    //copy assignment operator
    Deck& operator=(const Deck &other){
        //if assigning to the same object do nothing
        if(this == &other){
            return *this;
        }
        head = nullptr;
        tail = nullptr;
        cardNode * tmp = other.head;
        while(tmp){
            insertCard(tmp->number, tmp->suit);
            tmp = tmp->next;
        }
        return *this;
    }
/*
    int getValue(cardNode* card)
    {
        return card->number;
    }
    char getSuit(cardNode* card)
    {
        return card->suit;
    }
    */
    int getSize()
    {
        cardNode* tmp = head;
        int size = 0;
        while (tmp != nullptr)
        {
            size++;
            tmp = tmp->next;
        }
        return size;
    }
    //Function to create a standard deck of 52 cards in random order
    void createDeck()
    {
        while (!empty()) //clear old game deck if playing a new game
        {
            if (head != nullptr)
            {
                cardNode* tmp = head;
                head = head->next;
                if (head == nullptr)
                    tail = nullptr;
                delete tmp;
            }
        }

        for (int i = 0; i < 4; i++)
        {
            for (int j = 1; j < 14; j++)
            {
                if (i == 0)
                    insertCard(j, HEARTS);
                if (i == 1)
                    insertCard(j, SPADES);
                if (i == 2)
                    insertCard(j, DIAMONDS);
                if (i == 3)
                    insertCard(j, CLUBS);
            }
        }
        //randomize list
        //use random number generator to get random "index"
        srand(time(nullptr));
        //remove node and insert
        for(int i=0; i<100;i++){
            int randomIndex = rand() % 50 +1;
            cardNode *tmp = this->head;
            cardNode * previous = nullptr;
            for(int j = 0; j< randomIndex; j++){
                //increment tmp and previous
                previous = tmp;
                tmp = tmp->next;
            }
            previous->next = tmp->next;
            //insert the removed node in beginning of list
            insertCard(tmp->number, tmp->suit);
            delete tmp;
            previous = nullptr;
            delete previous;
        }
    }

    //deck only has to insert at the tail
    void insertCard(int value, char suit){
        cardNode * newCard = new cardNode(value, suit);
        if(head == nullptr){//empty deck
            head = tail = newCard;
        }else{//not empty deck
            //only need to append at the end of the deck
            tail->next = newCard;
            tail = newCard;
        }
    }
    //deck only has to remove from the head since players draw from top
    //but playerHands can remove from anywhere in the players group of cards
    // so removeCard will have to be overridden. This is why we are making
    // removeCard virtual.
    virtual bool removeCard() {
        //remove card from head only if not an empty list
        //remove card from head only if not an empty list
        if (head == nullptr){//empty list
            //do nothing
            return false;//can't remove, empty deck
        }else{//not empty list
            cardNode *tmp = head;
            head = head ->next;
            if(head == nullptr)//now an empty list
                tail = nullptr;
            delete tmp;
            return true;//was able to remove
        }
    }
    //need a peek function to know what the values of the card on the top are
    cardNode* peek(){//cant call peek on empty list
        //head is always at the top of the deck
        return head;
    }
    bool empty(){
        return this->head == nullptr;
    }
    void printList(){
        cardNode * tmp = head;
        while(tmp != nullptr){
            cout << tmp->number << tmp->suit << ", " ;
            tmp = tmp->next;
        }
        cout << endl;
    }

};

class PlayerHands: public Deck {
public:
    //default constructor
    PlayerHands(){
        head = nullptr;
        tail = nullptr;
    }

    //can use inherited insert

    //insert seven cards from deck into player hands
    void deal(Deck* globalDeck) {
        for(int i = 0; i < 7; i++){//insert seven times from deck
            int value = globalDeck->peek()->number;
            char suit = globalDeck->peek()->suit;
            //insert drawn card into player hands
            this->insertCard(value, suit);
            //remove drawn card from deck
            globalDeck->removeCard();
        }
    }
    //draw one card from deck, print value drawn
    bool draw(Deck* globalDeck) {
        //check if empty deck
        if(globalDeck->peek() == nullptr){
            cout << "empty deck" << endl;
            return false;
        }else{//not empty deck
            //create new card node to insert into players hands
            int value = globalDeck->peek()->number;
            char suit = globalDeck->peek()->suit;
            //insert drawn card into player hands
            this->insertCard(value, suit);
            //remove drawn card from deck
            globalDeck->removeCard();
            return true;
        }
    }

    //return card value of most recently drawn card in hand
    int checkNewCard()
    {
        if (tail != nullptr)
            return tail->number;
        else
            return -1;
    }
    //need to find Card from each player's hand
    //find function only looks at number value and not suit since
    //number value is what creates a match in GoFish
    bool find(int valueToFind) { //***Problem Here?
        //start iterating at beginning of players list
        cardNode * tmp = this->head;
        while(tmp != nullptr){
            if(tmp->number == valueToFind){
                //return true if found
                return true;
            }
            tmp = tmp->next;
        }
        //return false if not found
        return false;
    }

    //find majority card value in this player's hand
    int mostCommon()
    {
        cardNode* temp = head;

        //Store card values and their frequencies in hash table
        unordered_map<int, int> hash;
        int total = 0;

        while (temp != nullptr)
        {
            hash[temp->number]++; //increase card frequency
            temp = temp->next;
            total++;
        }

        //Check for card value majority
        for (auto i: hash)
        {
            if (i.second >= total/2)
                return i.first;
        }
        return -1; //no card value majority
    }

    //need to override inherited remove
    bool remove(int valueToRemove, char suitToRemove){
        //can remove from anywhere in linked list
        cardNode *tmp = this->head;
        cardNode * previous = nullptr;
        while(tmp->next != nullptr){//loop through list until node is found
            if((tmp->number == valueToRemove) &&
               (tmp->suit == suitToRemove)){
                //card found now remove it

                if(tmp == head) {//remove head
                    if (tmp == tail) {//removing last node in the list
                        head = tail = nullptr;
                    }else{
                        head = tmp->next;
                    }
                }else if(tmp == tail){//remove tail
                    tail = previous;
                    tail->next = nullptr;
                }else{//remove middle
                    previous->next = tmp->next;
                }
                delete tmp;
                return true;
            }
            //increment tmp and previous
            previous = tmp;
            tmp = tmp->next;
        }
        //delete nodes created
        delete tmp;
        previous = nullptr;
        delete previous;
        //return false if not found
        return false;
    }

    //Function to remove card from other's hand and insert into this Player's hand
    bool replace(PlayerHands* other, int value)
    {
        cardNode * tmp = other->head;
        char cardSuit;
        while(tmp->next != nullptr){
            if(tmp->number == value){//found
                //return true if found
                cardSuit = tmp->suit;
                other->remove(value, cardSuit);
                this->insertCard(value, cardSuit);
                delete tmp;
                return true;
            }
            tmp = tmp->next;
        }
    return false;
    }

    //Function to check if a hand has any books
    //If so, remove those cards and increment their score
    bool book(int value)
    {
        cardNode *curr = this->head;
        int count = 0;
        while (curr != nullptr)
        {
            if (curr->number == value)
            {
                count++;
            }
            curr = curr->next;
        }
        if (count == 4) //book is formed, remove cards from the hand
        {
            remove(value, SPADES);
            remove(value, DIAMONDS);
            remove(value, HEARTS);
            remove(value, CLUBS);
            return true;
        }
        return false;
    }
};

//A Player class represents a player in the game
//player is a playerHand
class Player: public PlayerHands {
protected:
    //PlayerHands* myHand;
    //player needs a score and id
    int playerID;
    int score;
public:
    //default constructor
    Player(int id = 0)
    {
        playerID = id;
        score = 0;

    }

    //Returns the playerID of this Player
    int getID()
    {
        return this->playerID;
    }

    //Returns the hand of this Player
    /*PlayerHands* getHand()
    {
        return myHand;
    }
*/
    int getScore()
    {
        return this->score;
    }

    //Function handling asking for an opponent's card
    //Takes input for human player's turn
    //Overriden in ConsolePlayer for CPU logic
    virtual bool ask(Player* other, Deck* globalDeck)
    {
        bool keepTurn = false;
        int response = -1;
        char moveOn;

        do { //receive user input
            cout << "What card would you like to ask for?" << endl;
            cin >> response;
            if (!this->find(response)) //***Allows asking for card we don't have
                cout << "Please choose a card in your hand." << endl;
        } while (!this->find(response));

        if (other->find(response)) //opponent has asked card, take
        {
            cout << "Your opponent has this card!" << endl;

            bool replaced = false;
            while(other->find(response)) //swap all the cards that match between hands //****Trouble
            {
                replaced = this->replace(other, response);
                cout << "Replaced card? " << replaced << endl;
            }
            cout << "TEST done replacing" << endl;
            keepTurn = true;
        }
        else //opponent does not have card, draw
        {
            cout << "Your opponent does not have this card, Go Fish!" << endl;
            this->draw(globalDeck);
            if (response == this->checkNewCard()) //if drawn card matches wish, keep turn
            {
                cout << "Congrats, you drew the card you wanted! Ask again!" << endl;
                keepTurn = true;
            }
        }
        if(this->book(response))
            this->score++;
        
        cout << "Press any key to continue " << endl;
            cin >> moveOn;
        return keepTurn;
    }

};

//A ConsolePlayer is a Player
//Overrides functions taking user input to handle use by CPUs
class ConsolePlayer : public Player {
public:
    ConsolePlayer(int id) : Player (id)
    {
    }

    bool ask(Player* other, Deck* globalDeck) //Computer logic for ask function
    {
        //Find card value with most occurrences in hand
        //Ask random player for that card
        bool keepTurn = false;
        int playerIndex = this->playerID + 1;
        string moveOn;

        int askValue = this->mostCommon(); //get most common card value in hand
        if (askValue < 0) //no card in hand has majority
        {
            askValue = this->peek()->number; //set value to ask to whatever is first card in hand
        }
        
        cout << "CPU " << playerIndex << " asks for " << askValue << "'s" << endl;

        if (other->find(askValue)) //opponent has asked card, take it
        {
            int test = 0;
            while(test < 1/*other->find(askValue)*/) //swap all the cards that match between hands //****Stuck in this loop
            {
                this->replace(other, askValue);
                cout << "TEST replaced one" << endl;
                test++;
            }
            cout << "TEST done replacing" << endl;
            keepTurn = true;
        } else //opponent does not have card, draw
        {
            cout << "Go Fish!" << endl;
            this->draw(globalDeck);
            if (askValue == this->checkNewCard()) //if drawn card matches wish, keep turn
            {
                cout << this->playerID << " drew their wish, they keep their turn!" << endl;
                keepTurn = true;
            }
        }
        if(this->book(askValue))
            this->score++;
        
        cout << "Press any key to continue " << endl;
            cin >> moveOn;
        return keepTurn;
    }

};


//Contains the functions to play the game
class Game {
public:
    Deck * gameDeckk;
    int numPlayers;
    int currentPlayer;
    bool isOver;            //checks game isn't over
    bool humanPlaying;

    vector<Player*> players;
    //default constructor (missing Maritza Added)
    Game(Deck* gameD = nullptr, int totalPlayers = 0, int CurrentPlayer = 0, bool isGameOver = false, bool isHumanPlaying = false){
        numPlayers = totalPlayers;
        currentPlayer = CurrentPlayer;
        isOver = isGameOver;
        humanPlaying = isHumanPlaying;
        gameDeckk = gameD;

    }
    //cant do all this in default constructor, should set up game in a function
    void gameSetUP()
    {
        ConsolePlayer* cpu1 = new ConsolePlayer(0);
        ConsolePlayer* cpu2 = new ConsolePlayer(1);

        char human;
        cout << "Is there a human playing? (y/n)" << endl;
        cin >> human;
        players.push_back(cpu1);
        cpu1->deal(gameDeckk);
        players.push_back(cpu2);
        cpu2->deal(gameDeckk);
        if (human == 'y' || human == 'Y')
        {
            numPlayers = 3;
            humanPlaying = true;
            Player* humanPlayer = new Player(2);
            players.push_back(humanPlayer);
            humanPlayer->deal(gameDeckk);
        }
        else
        {
            numPlayers = 2;
        }
    }

//Returns the Player currently taking their turn
    Player* currentlyPlayingPlayer()
    {
        return getPlayerAtIndex(currentPlayer);
    }

//Returns the Player at the given index in the players vector
    Player* getPlayerAtIndex(int index)
    {
        return players.at(index);
    }

//Function to end a given game
    void endGame()
    {
        int winner = 0;
        for (int i = 1; i < players.size(); i++)
        {
            if (players[i]->getScore() > players[winner]->getScore())
                winner = i;
            isOver = true;
        }
        cout << "Game Over!" << endl;
        cout << "Player " << winner << " won, congrats!" <<endl;
    }

//Function covering a Player's turn
//Decide whom to ask for a card
    void takeTurn(Player* playing, Deck* globalDeck)
    {
        bool valid = false;
        int playerToAsk;
        printGame();
        int playerPosition = playing->getID(); //Human-readable index of current player (i.e. index 0 == CPU 1)
        playerPosition++;

        if (humanPlaying) //3 players
        {
            if (playing->getID() == 2) //human player, get user input
            {

                do {
                    cout << "Player, select an opponent to ask." << endl;
                    cin >> playerToAsk;
                    playerToAsk--; //decrement value to match C++ indexing (1 -> 0)
                    if(playerToAsk == playing->getID() || (playerToAsk >= numPlayers || playerToAsk < 0))
                        cout << "Please choose a valid opponent" << endl;
                    else
                        valid = true;
                }  while (!valid);
            }
            else //CPU, randomly choose another player to ask
            {
                srand(time(nullptr));
                playerToAsk = rand() % 2;
                //CPU1 = 0, CPU2 = 1, Human = 2
                if ((playing->getID() == 0 && playerToAsk == 0) || (playing->getID() == 1 && playerToAsk == 1))
                    playerToAsk++;
                if (playerToAsk == 1)
                    cout << "CPU " << playerPosition << " asks CPU 2" << endl;
                else
                    cout << "CPU " << playerPosition << " asks the Player" << endl;
            }
        }
        else //2 cpu's, must always ask the other player
        {
            if (playing->getID() == 0)
            {
                playerToAsk = 1;
                cout << "CPU 1 asks CPU 2" << endl;
            }
            else
            {
                playerToAsk = 0;
                cout << "CPU 2 asks CPU 1" << endl;
            }
            
        }

        turn(playing, getPlayerAtIndex(playerToAsk), globalDeck);
    }

//***Function to print game state
    void printGame()
    {
        cout << endl;
        cout << "Current Game State" << endl;
        cout << endl;
        cout << "CPU1 has " << players[0]->getSize() << " cards" << endl;
        cout << "CPU2 has " << players[1]->getSize() << " cards" << endl;
        cout << endl;
        if (humanPlaying) //print Human's hand
        {
            cout << "You currently have: " << endl;
            players[2]->printList();
            cout << endl;
        }
    }


//Function covering a Player's turn
//Loop a Player's turn if they get to keep playing
//Otherwise, increment to next player
    void turn(Player* asking, Player* asked, Deck* globalDeck)
    {
        bool keepPlaying = asking->ask(asked, globalDeck);
        if (keepPlaying)
            takeTurn(asking, globalDeck);
        else
        {
            int nextID = asking->getID();
            nextID += 1; //increment to next player
            if (nextID < numPlayers) //if next player does not overflow players vector
                takeTurn(players[nextID], globalDeck);
            else //wrap back to start
                takeTurn(players[0], globalDeck);
        }
    }
};



int main ()
{
    //Computer Logic Loop
    //Print win condition
    //Print hand (in Player)
    //Initialize Players
    srand(time(nullptr));
    bool newGame = false;
    char goAgain;

    do {
        Deck* gameDeck = new Deck();
        gameDeck->createDeck();

        Game game(gameDeck);
        game.gameSetUP();
        do{
            game.takeTurn(game.currentlyPlayingPlayer(), gameDeck);
        }while(!game.isOver);

        cout << "Would you like to play again? (y/n)" << endl;
        cin >> goAgain;
        if (goAgain == 'y' || goAgain == 'n')
            newGame = true;
        else
            newGame = false;
    } while(newGame);

    return 0;
}