# CS50 Final Project
## Requirements Specification
 
> In a requirements spec, **shall do** means **must do**.
 
The Final Project uses the idea of reinforcement learning to create a program that learns to play the popular card game 21 (also known as blackjack) in an optimal manner. The program includes a dealer who facilitates each game and a player that will play hundreds, maybe thousands, of hands of 21 and learn which actions work well (e.g., the program wins), and which do not (e.g., the program loses). 
 
The dealer **shall**:
 
1. run from the command line, taking the Port Number and desired number of games as parameters
2. set up a server socket listening for clients (players) to connect
3. once a client connects, pass messages back and forth over socket as shown in diagram below, and
create a deck of 52 cards for each game
shuffle the deck
deal cards to the player by sending messages with the card suit and rank as a string (e.g., “Seven of Hearts”)
receive HIT/STAND decisions from the client
calculate the game’s result (WIN, LOOSE, or PUSH) and send a message to the client
reset and play again (you decide on how many games to play)
4. send a QUIT message to the client when done

The player **shall**:
 
1. run from the command line taking the player’s name, server’s IP address, and PORT number as parameters
2. connect to the server using a socket
3. have a training mode where it plays many games with the dealer program, choosing random actions
must be able to write its Q table to disk and read it back
must be able to continue training after reading the Q table from disk
4. have a play mode where it makes optimal decisions based on what it learned during training
reads Q table written to disk during training
uses table to make optimal decisions
 
**Definition**:

![image not found](https://www.cs.dartmouth.edu/~tjp/cs50/project/MessagePassing.png)

 * JOIN <player name>: player asks to join game, player name should not have spaces (use underscore for spaces e.g., team_one_is_here)
 * BEGIN: to keep dealer and player in sync, if a player gets a BEGIN message, they should reset for a new game (e.g., discard any cards)
 * CARD: dealer sends player a card, rank will be strings “Two” through “Ten”, “Jack”, “Queen”, “King”, or “Ace”, suit will be “Diamonds”, “Hearts”, “Clubs” or “Spades” (e.g., “CARD Nine of Hearts”)
 * DEALER: dealer tells player the Rank of Suit of dealer’s face-up card (e.g., “DEALER Ace of Clubs”)
 * DECISION: dealer asks the player to make a decision (either HIT or STAND)
 * HIT or STAND: player tells the dealer their decision based on player’s cards and dealer’s face-up card (repeat until STAND or BUST)
 * RESULT: dealer tells the player if they WIN, LOOSE, BUST, or PUSH (followed by BEGIN if playing multiple rounds)
 * QUIT: dealer tells player to quit
 
 
**Assumption**:
We assume that the player knows the syntax of message exchanging between the player and the dealer. If any sent message is not valid, the program just exits non-zero and does not ask the player/the user to re-enter their message. The player/user will have to re-run the program.
 
 
**Limitations**:
The Network module uses a delay when sending of 50 ms to prevent overlapping messages. This value was found experimentally.
 
 

