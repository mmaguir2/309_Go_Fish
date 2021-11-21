# 309_Go_Fish
Team Vector
Group members/contributers: 
Maritza Aguirre-Diaz, Spenser Geideman, Tyler Karpowich

How to compile and run your code. List any software or system dependences (Linux/Windows, etc).
Interesting features. 
Status report on the code. If something doesn’t work, explain why.


For this project we decided to model the card game 'Go Fish.' We implemented the game to allow either two computer players to play against each other such that they can be 'watched,' or to allow a human player to join in and play against these two CPUs. Our rule set is taken from Bicycle Cards (https://bicyclecards.com/how-to-play/go-fish/) and is as follows:

  - Use a standard 52 card deck of cards
    - In our case, face cards are represented by their numerical value (Ace = 1, Jack = 11, Queen = 12, King = 13) to simplify user input and computer logic
  - Players gain score by forming 'books' of all four cards of a given value (e.g. four kings: King of Spades, King of Hearts, King of Diamonds, and King of Clubs)
  - At the beginning of the game, 7 random cards are dealt to each player
  - During their turn, a player (asker) must ask another player (askee) for a specific card value (suits have no bearing on this game) that they have in their own hand (a player cannot ask for a card they do not currently possess)
  - If the askee has at least one of the cards asked for, the asker receives all copies of that card value and takes their turn again
  - If the askee does not have the card asked for, the asker draws one card from the deck and their turn passes to the next player
    - Unless the card drawn matches the value the asker wanted, they are 'granted their wish' and may take their turn again
  - When a player holds all four cards of a specific value in their hand, they announce the formation of a book, gain a point, and discard those cards
  - If at any point a player runs out of cards in their hand, they draw up to seven more cards from the deck to refill their hand (this does not take up their turn)
    - If there are fewer than seven cards in their deck, the drawing player takes all the remaining cards
    - If there are no more cards in the deck, the game ends
  - The game ends when all 13 books have been formed or when the deck is empty and a player has run out of cards
    - The player with the most books wins the game

There are no additional files needed to compile and run this code, nor any particular requirements for compilation. There are no software dependencies

The CPU logic in our game is relatively simplistic, but models a viable strategy for playing Go Fish. At the start of their turn, the CPU checks their hand and finds the most common card in their hand. They will then ask a random opponent (or the other CPU if there are only two players) for that card. If they don't have a card value that holds a plurality in their hand, they will ask for the first card in their hand.
