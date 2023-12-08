# CS50 Final Project
## Implementation Specification
 
In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.
Implementation may include many topics; not all are relevant to this project.
 
Here we focus on the core subset:
 
-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan
 
## Data structures
 
The key data structure is the *Q table* which is a 3D array that holds all possible states (comprised of cards the player has and the face-up card the dealer holds) and each possible action (HIT or STAND). This table will be used to store the average reward received after taking each action in each state.
 
In the *network* module:
- *server* struct that holds relevant server data (eg. port, address, etc.)
- *client* struct that holds relevant client data (eg. port, server address, etc.)
 
In the *cards* module:
- *hand* struct is an array that holds a number of cards
- *deck* struct is a *hand* that holds 52 cards
 
 
## Control flow
 
The final project is implemented in two main files:
* `dealer.c` - the server, handles gameflow
* `player.c` - the client, handles training, playing, and reinforcement learning
 
### Dealer Functions
### 1. main
 
The `main` function is responsible for running the whole game.

Pseudocode:

```
parse the command line, validate parameters, initialize other modules
   start server in the port number provided by the user
   store the number of game rounds
   while we have not reached the final game round:
       initialize a new deck
	shuffle the deck
	Send a BEGIN message to the player
	Deal the first card to the player
	Deal the second card to the player
	Deal the first card to the dealer face up
Deal the second card to the dealer face down
	ask the the player for their decision of whether to HIT or STAND
		while the player’s decision is not STAND
			If the player’s decision is HIT
				Deal the player a card
				Calculate the current hand value of the player
If the player busts
					break
				Else
					Ask for their next decision
	Calculate the player hand value
	Calculate the dealer hand value
	If the dealer hand value is less than 17
		Deal a card to the dealer until they are equal or above 17
	Once the dealer is equal or above 17
		if the dealer hand value > 21
	The dealer busts and the player wins
Calculate the result of the game and send it to the player
Send the game result to the player
Send QUIT to the player
```
 
### 2. printUsage
 
Prints out to a passed (FILE* fp) the usage of the dealer program.
The usage is: 
```bash
./dealer portNumber numberOfRounds
```
 
### 3. calculateGameResult
 
Passing the dealer hand and the player hand, calculateGameResult returns the result of the game based on the value of the two hands passed to the function.
Pseudocode:
```
	Calculate the value of the player hand
	Calculate the value of the dealer hand
	If player value > 21 player loses
	If dealer value > 21 player wins
	If dealer value = player value player pushes
	If player value > dealer value player wins
	If player value < dealer value player loses
 ```
 
### Player Functions

### 1. Play
```
play_game(Q, client)
	serverMessage = readServerMessage(client)
	Verify that serverMessage == "BEGIN"
	If not, return 1.
 
	playerHand = hand_new()
 
	serverMessage = readServerMessage(client)
	Verify that the first 4 letters of serverMessage == "CARD"
	If not, free memory and return 1. 
 
	card_t playerCard1 = read_card(serverMessage, 4)
	Add playerCard1 to playerHand
 
	serverMessage = readServerMessage(client)
	Verify that the first 4 letters of serverMessage == "CARD"
	If not, free memory and return 1. 
 
	card_t playerCard2 = read_card(serverMessage, 4)
	Add playerCard2 to playerHand
	
	serverMessage = readServerMessage(client)
	Verify that the first 6 letters of serverMessage == "DEALER"
	If not, free memory and return 1.
 
	card_t dealerCard = read_card(serverMessage, 6)
	int dealerCardValue = card_getValue(dealerCard)
	
	serverMessage = readServerMessage(client)
	Verify that serverMessage == "DECISION"
	If not, free memory and return 1.
	
	double hit = Q[hand_getValue(playerHand)][dealerCardValue][0]
	double stand = Q[hand_getValue(playerHand)][dealerCardValue][1]
 
	if hit > stand set action to 0 else to 1.
 
	if action == 0: sendToServer(client, "HIT")
	else: sendToServer(client, "STAND")
 
	while action == 0:
		serverMessage = readServerMessage(client)
		Verify the first 4 letters of serverMessage == "CARD"
		If not, free memory and return 1. 
		card_t playerCard = read_card(serverMessage, 4)
		Add playerCard to playerHand
 
		Break if hand_getValue(playerHand) > 21.
 
		serverMessage = readServerMessage(client)
		Verify that serverMessage == "DECISION"
		If not, free memory and return 1.
 
		double hit = Q[hand_getValue(playerHand)][dealerCardValue][0]
		double stand = Q[hand_getValue(playerHand)][dealerCardValue][1]
 
		if hit > stand set action to 0 else to 1.
 
 
		if action == 0: sendToServer(client, "HIT")
		else: sendToServer(client, "STAND")
 
		serverMessage = readServerMessage(client)
		Verify that first token of string == "RESULT"
		print the result of the game
	
	return 0	
 ```

### 2. Train

```
play_game_train(Q, Q_count, client, round, number_of_rounds)
	serverMessage = readServerMessage(client)
	Verify that serverMessage == "BEGIN"
	If not, return 1.
 
	playerHand = hand_new()
 
	serverMessage = readServerMessage(client)
	Verify that the first 4 letters of serverMessage == "CARD"
	If not, free memory and return 1. 
 
	card_t playerCard1 = read_card(serverMessage, 4)
	Add playerCard1 to playerHand
 
	serverMessage = readServerMessage(client)
	Verify that the first 4 letters of serverMessage == "CARD"
	If not, free memory and return 1. 
 
	card_t playerCard2 = read_card(serverMessage, 4)
	Add playerCard2 to playerHand
	
	serverMessage = readServerMessage(client)
	Verify that the first 6 letters of serverMessage == "DEALER"
	If not, free memory and return 1.
 
	card_t dealerCard = read_card(serverMessage, 6)
	int dealerCardValue = card_getValue(dealerCard)
	
	serverMessage = readServerMessage(client)
	Verify that serverMessage == "DECISION"
	If not, free memory and return 1.
	
	double hit = (double)rand() / (double)RAND_MAX which will give us a random number between 0 and 1.
	double stand = 1- hit.
 
	if hit > stand set action to 0 else to 1.
 
	int stateCount = 0 which is our counter for states
	state_t *states = malloc(sizeof(state_t) * 50) which is our array of states encountered
	states[stateCount] = currState which is initialized as {hand_getValue(playerHand), dealerCardValue, action}.
	stateCount++
 
	if action == 0: sendToServer(client, "HIT")
	else: sendToServer(client, "STAND")
 
	while action == 0:
		serverMessage = readServerMessage(client)
		Verify the first 4 letters of serverMessage == "CARD"
		If not, free memory and return 1. 
		card_t playerCard = read_card(serverMessage, 4)
		Add playerCard to playerHand
 
		Break if hand_getValue(playerHand) > 21.
 
		serverMessage = readServerMessage(client)
		Verify that serverMessage == "DECISION"
		If not, free memory and return 1.
 
		double hit = (double)rand() / (double)RAND_MAX
		double stand = 1- hit.
 
		if hit > stand set action to 0 else to 1.
 
		states[stateCount] = currState which is initialized as {hand_getValue(playerHand), dealerCardValue, action}.
		stateCount++
 
		if action == 0: sendToServer(client, "HIT")
		else: sendToServer(client, "STAND")
 
		serverMessage = readServerMessage(client)
		Verify that first token of string == "RESULT"
		Update totalReward based on the result of the game.
		
		for i = 0 to stateCount:
			update_Q with the current state and totalReward	
	return 0	
 ```
 
## Other modules
 
### Cards
 
We create a module `cards.c` to handle functionalities with regards to the playing cards used in the game. We choose to create this module separately for ease of testing and anticipating that it will be used by the dealer and player.
 
We implement each card as an integer from 1-52. A hand holds an array of cards and a deck is essentially a hand holding 52 cards at the beginning.
 
### 1. hand_new

Create a new (empty) hand.

Pseudocode for `hand_new`:
 
```
	Malloc the size of a hand
	Set “top” of hand to 0
	Return hand
```
 
### 2. hand_delete

Deletes a hand.

Pseudocode for `hand_delete`:
 
```
	Check if hand is valid
	Free hand
```
 
### 3. deck_new

Create a new card deck of 52 cards.

Pseudocode for `deck_new`:
 
```
	Make a new hand
	Initialize deck with cards 1-52
	Set “top” of deck to 52
```

### 4. deck_delete

Deletes a deck.

Pseudocode for `deck_delete`:
 
```
	Check if deck is valid
	Cast hand to deck and call hand delete
```
 
### 5. card_deal

adds a card from the deck to the given hand

Pseudocode for `card_deal`:
 
```
Check that deck and hand are valid
	If deck is not empty
		Draw a card from the “top” of the deck
		Add this card to hand
		Decrement pointer to “top” of deck 
		Increment pointer to “top” of hand
```
 
### 6. deck_shuffle

shuffles the deck.

Pseudocode for `deck_shuffle`:
 
```
	Check that deck is valid
	If deck is not empty
		Set seed for rand()
		Loop through every index in deck
			Pick a random index
			Swap the cards at the two indexes with helper function
		Return shuffled deck
 
```
 
### 7. swap

local function to swap two cards

Pseudocode for `swap`:
 
```
	Declare and initialize a temp card struct holding the value of card a
	Set value of card a to value of card b
	Set value of card b to the value of temp
 
```

### 8. deck_isEmpty

checks if deck is empty
 
Pseudocode for `deck_isEmpty`:
 
```
	Check if deck is valid
	If number of cards in a deck is zero
		Return true
	Else return false
 
```
 
### 9. card_getValue

gets a card's value

Pseudocode for `card_getValue`:
 
```
	Check if card is valid
	If the value of card mod 13 is greater than 10
		Return 10
	Else if the value of card mod 13 is 0
		Return 10
	Else if the value of card mod 13 is 1
		Return 1
 
```

### 10. hand_numAces

get number of Aces in a hand

Pseudocode for `hand_numAces`:
 
```
	Check if hand is valid
	Initialize count to 0
	Loop every card in hand
		If value of the card is 11
			Increment count by 1
	Return count
 
```

### 11. hand_getValue

gets a hand's total value

Pseudocode for `hand_getValue`:
 
```
	Check if hand is valid
	Initialize sum to 0
	Loop through every card in hand
		Add value of the card to sum
	While sum is greater than 21 and there are Aces in the hand
		Decrement sum by 10 (flips Ace to a 1)
		Decrement number of Aces by 1
	Return sum
 
```
### 12. hand_add

adds a card to hand

Pseudocode for `hand_add`:
 
```
	Check if hand and card are valid
	Add card to “top” of hand
	Increment pointer to “top” of hand
 
```

### 13. card_toString

returns a string holding the card and its suit

Pseudocode for `card_toString`:
 
```
	Check if card is valid
	Initialize a string to hold the cardName
	Initialize filler 
	Match the int value of card to its corresponding value in string
		Concatenate this string to cardName
	Concatenate filler to cardName
	Match the int value of card to its corresponding suit in string
		Concatenate this string to cardName
	Return cardName
 
```

### 14. string_toCard


Pseudocode for `string_toCard`:
 
```
	Record length of string
	Declare a words array of 3 strings
	Initialize index to 0
	Initialize boolean tracking if a space is seen before as true
	While index is less than 3
		Loop all the characters in string
			If character is a letter and there is a space before
				Set words[index] pointing at this string location
				Increment index by 1
				Set isSpaceBefore as false
			Else if character is a space
				Set this string location to a null character
				Set isSpaceBefore as true
	Loop all the words
		Loop all the letters in each word
			Turn each letter to lowercase
	Initialize multiplier to 0
	If the third word is “clubs”
		Set multiplier to 0
	If the third word is “diamonds”
		Set multiplier to 1
	If the third word is “hearts”
		Set multiplier to 2
	If the third word is “spades”
		Set multiplier to 3
	Initialize variable to track the face value of the card (1-13)
	Compare the first word with ace-king and record the corresponding face values
	Return 13 * multiplier * face value of card casted to a card struct
 
```

### 16. hand_print

prints all values in hand to file provided

Pseudocode for `hand_print`:

```
	Check if file and hand are valid
	If hand is not empty
	Loop every card in hand
		Print each card (each card is a value from 1-52)
```
### 17. deck_print

prints all values in deck to file provided

Pseudocode for `deck_print`:
 
```
	Cast deck to a hand and pass it to hand_print
```
 
   
### network
 
We create a network module that handles all network functionality required for the program. The network module utilizes TCP/IP sockets to establish connection. We chose to create this module to encapsulate the network abilities in one place, allowing for easier testing, debugging, and compartmentalization. The network module is composed of 9 functions.
 
### 1. startServer
 
Sets up server socket to listen and connect to clients.
 
Pseudocode:
 
```
server = malloc(sizeof(server_t))
Create socket file descriptor
Attach socket to the port
Bind socket to the port
Listen for connections
Accept connection, if available
Return server
```
 
### 2. readClientMessage
 
Read incoming message from client
 
Pseudocode:
 
```
Read incoming message into a buffer
Copy buffer into a string
Return message
```
 
### 3. sendToClient
 
Send message to a client
 
Pseudocode:
 
```
If isValidMessage
	Send message
	Delay for 50 ms
	Return true
Return false
```
### 4. closeServer
 
Clean up and end server.
 
Pseudocode:
 
```
Close connected socket
Shut down listening socket
free(server)
```
 
### 5. startClient
 
Sets up client socket to connect to server
 
Pseudocode:
 
```
client = malloc(sizeof(client_t))
Create socket
Convert server address to binary
Connect to server
Return client
```
 
### 6. readServerMessage
 
Read incoming message from server
 
Pseudocode:
 
```
Read incoming message into a buffer
Copy buffer into a string
Return message
```
 
### 7. sendToServer
 
Send message to a server
 
Pseudocode:
 
```
If isValidMessage
	Send message
	Delay for 50 ms
	Return true
Return false
```
 
### 8. closeClient
 
Clean up and end the client.
 
Pseudocode:
 
```
Close connected socket
free(server)
```
 
### 9. isValidMessage
 
Check if message is valid for client or server to send
 
Pseudocode:
 
```
Copy message into char* firstWord
Iterate through firstWord
	If found a space
		Change it to a null terminator
		Break
If message is coming from server
	For each valid one-word server message
		If message is equivalent to the message, result is true
	If firstWord == CARD or DEALER
		Scan message to grab rank and suit for “CARD/DEALER <rank> of <suit>”
		Iterate through suits
			If suit is valid, validSuit = true
		If validSuit
			Iterate through ranks
				If rank is valid, result = true
	If firstWord == RESULT
		Scan message to grab nextWord for “RESULT <result>”
		If result == WIN, LOOSE, or PUSH
			Result = true
If message is coming from client
	Iterate through valid one-word client messages
		If message matches, result is true
	If firstWord == JOIN
		Scan message to grab playername for “JOIN <playername>”
		If successful scan, result is true
free(firstWord)
Return result
```
 
## Function prototypes
 
### dealer
 
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `dealer.c` and is not repeated here.
 
```c
int main(const int argc, char* argv[]);
void printUsage(FILE* fp);
char* calculateGameResult(hand_t* playerHand, hand_t* dealerHand);
```
 
### player
 
#### qtable
```c
typedef struct state {
   int handValue;
   int dealerCardValue;
   int action;
} state_t;
 
void initialize_Q(float ***Q, int ***Q_count, int max_player_points, int max_dealer_points, int number_actions);
void update_Q(float ***Q, int ***Q_count, int player_points, int dealer_points, int action, int reward);
 
void save_Q(float ***Q, int max_player_points, int max_dealer_points, int number_actions);
 
void load_Q(float ***Q, int max_player_points, int max_dealer_points, int number_actions);
 
void free_Q(float ***Q, int ***Q_count, int max_player_points, int max_dealer_points, int number_actions);
```
 
#### training
```c
card_t read_card(char *serverMessage);
void play_game_train(float ***Q, int ***Q_count, char *server_address, int PORT);
void training(char *server_address, int PORT);
```
 
#### player

```c
int play_game(float ***Q, int ***Q_count, client_t *client)
void player(int argc, char *argv[])
```
 
### network
 
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `network.c` and is not repeated here.
 
```c
server_t* startServer(const int PORT);
char* readClientMessage(server_t* server);
bool sendToClient(server_t* server, char* message);
void closeServer(server_t* server);
client_t* startClient(const char *server_addr, const int PORT);
char* readServerMessage(client_t* client);
bool sendToServer(client_t* client, char* message);
void closeClient(client_t* client);
static bool isValidMessage(char* message, char* type);
int nanosleep(const struct timespec *req, struct timespec *rem);
```
 
### cards
 
Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `cards.h` and is not repeated here.
 
```c
typedef int card_t;
typedef struct hand hand_t;
typedef hand_t deck_t;
deck_t* deck_new(void);
void deck_delete(deck_t* deck);
hand_t* hand_new(void);
void hand_delete(hand_t* hand);
card_t card_deal(deck_t* deck, hand_t* hand);
deck_t* deck_shuffle(deck_t* deck);
bool deck_isEmpty(deck_t* deck);
int card_getValue(card_t card);
int hand_numAces(hand_t* hand);
int hand_getValue(hand_t* hand);
hand_t* hand_add(hand_t* hand, card_t card);
char* card_toString(card_t card);
card_t string_toCard(char* name);
void hand_print(hand_t* hand, FILE* fp);
void deck_print(deck_t* deck, FILE* fp);
static void swap(card_t* a, card_t* b);
 
```
 
 
## Error handling and recovery
 
All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.
 
Out-of-memory errors are handled by variants of checking dynamically-allocated memory for NULL, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.
 
All code uses defensive-programming tactics to catch and exit, e.g., if a function receives bad parameters.
 
## Testing plan
 
Here is an implementation-specific testing plan.
 
### Unit testing
 
There are four units in this project: dealer, player, network, and cards
 
That being said, dealer and player run on a server-client relationship, so unit testing is not possible (without effectively writing a dealer/player simulator). As a result, unit tests were conducted for the network and cards modules. However, we unit-tested the dealer module by checking and validating the parameters passed.

dealer:

The dealer module was tested by `testing.sh`. It passes one argument and then passes three arguments. It also passes a non-integer port number and a non-integer number of rounds.
 
network:

The network module was tested by creating `clientTest.c` and `serverTest.c`, client and server test drivers respectively. These programs simulate gameflow by: starting a server, starting a client, sending and receiving messages between each other, and closing the server and client. First, the tests simulate regular game play, passing valid messages that would be seen in actual gameplay from start to finish. Then, each program tests erroneous messages including server messages from the client, client messages from the server, invalid message structure (eg. CARD Jack Hearts), and typos.
 
In order to run the unit test, navigate into the `network` directory and start serverTest using: `./serverTest <PORT>` . Then start clientTest using: `./clientTest <server_addr> <PORT>`
Note: `wget -O - -q https://checkip.amazonaws.com` allows you to access IP Address

cards:

The cards module was tested by creating *cardsTest.c*. It calls every function in the module and uses print statements to check the accuracy of the outputs. Tests should:
1. Create a new deck
2. Deal the cards in this deck out to two hands
3. Print each card and its value and print the value of both hands
4. Check Ace cases
5. Simulate card communication between dealer and player (turning cards to strings and strings to cards)
 
 