//ECE 309 Go Fish Project
//Spenser Geideman
//Main function

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

/* TO DO
Create Deck of 52 randomized cards
Function in Game to print game State
Main Function
Decide on end game state
*/

using namespace std;

//define face cards
#define JACK    11
#define QUEEN   12
#define KING    13
#define ACE     1
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

    int getValue(cardNode* card)
    {
        return card->number;
    }
    char getSuit(cardNode* card)
    {
        return card->suit;
    }

    //***Function to create a standard deck of 52 cards in random order
    void createDeck()
    {
        while (!empty()) //clear old game deck if playing a new game
        {
            if (head != NULL)
            {
                cardNode* tmp = head;
                head = head->next;
                if (head == NULL)
                    tail = NULL;
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
        //Randomize Deck
        return;
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
        while(tmp){
            cout << tmp->number << tmp->suit << ", " ;
            tmp = tmp->next;
        }
        cout << endl;
    }

};

class playerHands: public Deck {
public:
    //default constructor
    playerHands(){
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
    
    //***return card value of most recently drawn card in hand
    int checkNewCard()
    {
        if (tail != NULL)
            return tail->number;
        else
            return -1;
    }
    //need to find Card from each players hand
    //find function only looks at number value and not suit since
    //number value is what creates a match in GoFish
    bool find(playerHands* otherPlayer, int valueToFind) {
        //start iterating at beginning of players list
        cardNode * tmp = otherPlayer->head;
        while(tmp->next != nullptr){
            if(tmp->number == valueToFind){
                //return true if found
                delete tmp;
                return true;
            }
            tmp = tmp->next;
        }
        //return false if not found
        return false;
    }

    //***find majority card value in this player's hand
    int mostCommon()
    {
        cardNode* temp = head;

        //Store card values and their frequencies in hash table
        unordered_map<int, int> hash;
        int total = 0;

        while (temp != NULL)
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

    //***Function to remove card from other's hand and insert into this Player's hand
    void replace(playerHands* other, int value)
    {
        cardNode * tmp = other->head;
        char cardSuit;
        while(tmp->next != nullptr){
            if(tmp->number == value){
                //return true if found
                cardSuit = tmp->suit;
                delete tmp;
            }
            tmp = tmp->next;
        }

        other->remove(value, cardSuit);
        this->insertCard(value, cardSuit);
    }

    //***Function to check if a hand has any books
    //If so, remove those cards and increment their score
    bool book(int value)
    {
        cardNode *curr = this->head;
        int count = 0;
        while (curr != NULL)
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

//Contains the functions to play the game
class Game {
public:
    int numPlayers;
    int currentPlayer = 0;
    vector<Player*> players;
    ConsolePlayer* cpu1 = new ConsolePlayer(1);
    ConsolePlayer* cpu2 = new ConsolePlayer(2);
    Game()
    {
        char human;
        cout << "Is there a human playing? (y/n)" << endl;
        cin >> human;
        if (human == 'y' || human == 'Y')
        {
            numPlayers = 3;
            Player* humanPlayer = new Player(0);
            players.push_back(humanPlayer);
        }
        else
        {
            numPlayers = 2;
        }
        players.push_back(cpu1);
        players.push_back(cpu2);
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

//***Function to end a given game
void endGame() 
{
    int winner = 0;
    for (int i = 1; i < players.size(); i++)
    {
        if (players[i]->getScore() > players[winner]->getScore())
            winner = i;
    }
    cout << "Game Over!" << endl;
    cout << "Player " << winner << " won, congrats!" <<endl;
}

//Function covering a Player's turn
//Decide whom to ask for a card
void takeTurn(Player* playing) 
{
    bool valid = false;
    int playerToAsk;
    do {
    cout << "Player " << playing->getID() << ", select a player to ask." << endl;
    cin >> playerToAsk;
    if(playerToAsk == playing->getID() && (playerToAsk >= numPlayers || playerToAsk < 0)) 
        cout << "Please choose a valid player" << endl;
    else
        valid = true;
    } while (!valid);
    
    turn(playing, getPlayerAtIndex(playerToAsk));
}

//***Function to print game state
void printGame()
{
    return;
}


//Function covering a Player's turn
//Loop a Player's turn if they get to keep playing
//Otherwise, incrment to next player
void turn(Player* asking, Player* asked) 
{
    bool keepPlaying = asking->ask(asked);
    if (keepPlaying)
        takeTurn(asking);
    else
    {
        int nextID = asking->getID();
        nextID += 1; //increment to next player
        if (nextID < numPlayers) //if next player does not overflow players vector
            takeTurn(players[nextID]);
        else //wrap back to start
            takeTurn(players[0]);
    }
}
};

//A Player represents a player in the game
class Player {
protected:
  //Game* game;
    playerHands* myHand;
    int playerID;
    int score;
    public:
    Player(int id = 0)
    {
        //this->game = game;
        playerID = id;
        score = 0;
        myHand = new playerHands();
        myHand->deal(gameDeck);
    }

    //Returns the playerID of this Player
    int getID()
  {
      return this->playerID;
  }

    //Returns the hand of this Player
    playerHands* getHand()
  {
    return myHand;
  }

    int getScore()
    {
        return score;
    }
    
    //***Function handling asking for an opponent's card
    //Takes input for human player's turn
    //Overriden in ConsolePlayer for CPU logic
    virtual bool ask(Player* other)
    {
        bool keepTurn = false;
        int response = -1;

        do { //receive user input
        cout << "What card would you like to ask for?" << endl;
        cin >> response;
        if (!myHand->find(myHand, response))
            cout << "Please choose a card in your hand." << endl;
        } while (!myHand->find(myHand, response));

        if (other->getHand()->find(other->getHand(), response)) //opponent has asked card, take
        {
            cout << "Your opponent has this card!" << endl;

            while(other->getHand()->find(other->getHand(), response)) //swap all the cards that match between hands
            {
                myHand->replace(other->getHand(), response);
            }
            keepTurn = true;
        } 
        else //opponent does not have card, draw
        {
            cout << "Your opponent does not have this card, Go Fish!" << endl;
            myHand->draw(gameDeck);
            if (response == myHand->checkNewCard()) //if drawn card matches wish, keep turn
            {
                cout << "Congrats, you drew the card you wanted! Ask again!" << endl;
                keepTurn = true;
            }
        }
        if(myHand->book(response))
            this->score++;
        return keepTurn;
    }

};

//***A ConsolePlayer is a Player
//Overrides functions taking user input to handle use by CPUs
class ConsolePlayer : public Player {
public:
  ConsolePlayer(int id) 
  {
    Player (id);
  }
  
  bool ask(Player* other) //Computer logic for ask function
  {
    //Find card value with most occurrences in hand
    //Ask random player for that card
    bool keepTurn = false;

    int askValue = myHand->mostCommon(); //get most common card value in hand
    if (askValue < 0) //no card in hand has majority
    {
        askValue = myHand->peek()->number; //set value to ask to whatever is first card in hand
    }

    if (other->getHand()->find(other->getHand(), askValue)) //opponent has asked card, take it
    {
        while(other->getHand()->find(other->getHand(), askValue)) //swap all the cards that match between hands
        {
            myHand->replace(other->getHand(), askValue);
        }
        keepTurn = true;
    } else //opponent does not have card, draw
    {
        cout << "Your opponent does not have this card, Go Fish!" << endl;
        myHand->draw(gameDeck);
        if (askValue == myHand->checkNewCard()) //if drawn card matches wish, keep turn
        {
            cout << this->playerID << " drew their wish, they keep their turn!" << endl;
            keepTurn = true;
        }
    }
    if(myHand->book(askValue))
            this->score++;
    return keepTurn;
  }

};



Deck* gameDeck = new Deck();

void main ()
{
    //Computer Logic Loop
    //Print win condition
    //Print hand (in Player)
    //Initialize Players
    bool newGame = false;
    char goAgain;

    do {
        gameDeck->createDeck();
        


        cout << "Would you like to play again? (y/n)" << endl;
        cin >> goAgain;
        if (goAgain == 'y' || goAgain == 'n')
            newGame = true;
        else   
            newGame = false;
    } while(newGame);
}