# CS50 Final Project
## Design Specification
 
In this document we reference the [Requirements Specification](REQUIREMENTS.md) and focus on the implementation-independent design decisions.  Here we focus on:
 
- User interface
- Inputs and outputs
- Functional decomposition into modules
- Pseudo code (plain English-like language) for logic/algorithmic flow
- Major data structures
- Testing plan
 
## User interface
 
### Dealer
As described in the [Requirements Spec](REQUIREMENTS.md), the program's only interface with the user is on the command-line; it must always have two arguments.
 
```bash
$ dealer portNumber numberOfRounds
```
 
For example, to play 5 rounds of blackjack using the port 8088, use this command line:
 
``` bash
$ ./dealer 8088 5
```
 
### Player
Player has 2 interactive submodules. training is for filling the q table and player is for playing the game using what’s been learned during training
 
We selected 50000 rounds to train our agent. This is an estimate as we started training 1000 and 10000 rounds and weren’t satisfied with results. A good heuristic could be to calculate difference between prior state and stop training once the difference reaches a certain threshold.
 
#### Training
```bash
./training 0.0.0.0 8088 50000
```
This command trains the agent using 50000 rounds of game.
 
#### player
```bash
./player 0.0.0.0 8088
```
This command connects the player to server using the ip address and port and starts playing using the qtable. We expect that the file “qtable.txt” already exists in the same directory as player.
 
 
## Inputs and outputs
 
*Input:* there are no file inputs; there are command-line parameters described above.
 
*Output:* Per the [Requirements spec](REQUIREMENTS.md), in the training mode, the player will save the stages and rewards of each game round into a table. In the playing mode, the player uses the table as a reference to make decisions of whether to HIT or STAND.
Within each training round, the player writes into the Q table each stage of the game as an independent game but then adds the same reward to each stage because they all contributed to the same reward.
 
## Functional decomposition into modules
 
We anticipate the following modules or functions:
 
1. *dealer*, which basically runs the game in general. The dealer deals cards to the player and to themselves, and then waits for decisions from the player until the player STANDs. Then, the dealer calculates the result of the game and sends it back to the player. The dealer restarts the game every time we start a new round. The dealer repeats playing for the same number of rounds entered by the user.
 
2. *player*, which comprises 3 modules: qtable, training, and player. qtable handles initializing Q and Q_count, loading and updating Q and Q_count, and freeing Q and Q_count. training runs the game some number of times and fills out the Q table, and the player plays the game using information learned via Q table.
 
3. *cards*, which creates structs holding a deck of 52 cards, a hand, a card. The cards module allows the user to interact with a deck of cards through functions that shuffle and deal cards as well as functions that return information about the cards used in the game.
 
4. *network*, which serves to compartmentalize client-server functionality to be used by both the *player* and *dealer* modules. This module allows the dealer (server) to communicate with the player (client). 
 
## Pseudo code for logic/algorithmic flow
 
The dealer will run as follows:
 
```
    parse the command line, validate parameters, initialize other modules
    start server in the port number provided by the user
    store the number of game rounds
    while we have not reached the final game round:
       initialize a new deck
	shuffle the deck
	deal two cards face up to the player
	deal two cards to the dealer, one face up and the other face down
	ask the the player for their decision of whether to HIT or STAND
		while the player’s decision is not STAND
			If the player’s decision is HIT
				Deal the player a card
				If the player busts
					break
				Else
					Ask for their next decision
	Calculate the player hand value
	Calculate the dealer hand value
	If the dealer hand value is less than 17
		Deal a card to the dealer until they are equal or above 17
	Once the dealer is equal or above 17
		Calculate the result of the game and send it to the player

```


The training agent will run as follows:
 
```
play_game_train(Q, Q_count, client, round, number_of_rounds)
 
	receive the "BEGIN" message using client
	create a hand for the player
	
	receive the "CARD" message using client
	add the card to hand
	
	receive the "CARD" message using client
	add the card to hand
	
	receive the "DEALER" message using client
	save card's value to dealerCardValue
 
	receive the "DECISION" message using client
	set hit to random double between 0 and 1
	set stand to 1 - hit
	
	set action to 0 if hit > stand else 1
 
	create a state and add it to states
 
	send action to server
 
	while action = 0:
		receive the "CARD" message using client
		add the card to hand
		
		if value of hand is > 21, break the loop
		
		receive "DECISION" message using client
		
		set hit to random double between 0 and 1
		set stand to 1 - hit
	
		set action to 0 if hit > stand else 1
 
 
		create a state and add it to states
 
		send action to server
		if action is 1, then break the loop
 
	receive the "RESULT" message using client
	set totalReward to 1 if "WIN", -1 if "LOOSE", 0 if "PUSH"
	
	loop through states:
		update_Q(Q, Q_count, handValue, dealerCardValue, action, totalReward)
 
	return 0;
 
 ```
 
The player will run as follows:
 
```

player(argc, argv)
	check the number of parameters is 3
	start a client using argv[1] and argv[2]
	
	Allocate space for Q
	Open qtable.txt file to load the Q table
 
	Call play_game(Q, client)
	close the client and return 0
 
play_game(Q, client)
	Send "JOIN player1" to server using client
	Read "BEGIN" message
 
	Create the player's hand
 
	Read first "CARD" message and add it to hand
	Read second "CARD" message and add it to hand
 
	Read "DEALER" card message and save its value to dealerCardValue
	
	Read "DECISION" message
	
	Get rewards for HIT and STAND using current state in Q table
	Set action to 0 if hit > stand else 1
	Send the appropriate message to client ("HIT" or "STAND")
 
	While we can take another card:
		read "CARD" message and add it to hand
		if hand value exceeds 21, break the loop
		
		read "DECISION" message
 
		Get rewards for HIT and STAND using current state in Q table
		Set action to 0 if hit > stand else 1
		Send the appropriate message to client ("HIT" or "STAND")
	
	Read "RESULT" message
```

## Major data structures
 
float ***Q = malloc(sizeof(float **) * 22);
int ***Q_count = malloc(sizeof(int **) * 22);
 
Q and Q_count tables are dynamically allocated 3D arrays with dimension 22x22x3. It’s an intentional choice to add extra 1 to each of the three dimensions.
 
They are initialized as following:
```c
void initialize_Q(float ***Q, int ***Q_count, int max_player_points, int max_dealer_points, int number_actions) {
    int i, j, k;
    for (i = 0; i < max_player_points; i++) {
        Q[i] = malloc(sizeof(float *) * max_dealer_points);
        Q_count[i] = malloc(sizeof(int *) * max_dealer_points);
        for (j = 0; j < max_dealer_points; j++) {
            Q[i][j] = malloc(sizeof(float) * number_actions);
            Q_count[i][j] = malloc(sizeof(int) * number_actions);
            for (k = 0; k < number_actions; k++) {
                Q[i][j][k] = 0;
                Q_count[i][j][k] = 0;
            }
        }
    }
}
```
 
 
Helper modules provide all the data structures we need:
 
 
The *cards* module holds a card struct, hand struct, and deck struct. It supports the creation and removal of the deck struct and hand struct as well as functions that will be called by the dealer or player to perform some action on the cards or return some information about the cards. 
 
 
## Testing plan
 
In the *dealer* module, we used the *nc 0.0.0.0 [portNumber]* command to connect to the port the dealer was listening to. After connecting to the same socket, we were testing the dealer by sending messages from the player-simulating terminal like *JOIN player1* and *HIT*, and *STAND*. We tested edge cases to make sure the dealer hands them well. However, whenever an invalid message is sent to the dealer, the dealer exits non-zero.
We also provided a testing.sh file to test arguments passed to the dealer. Check IMPLEMENTATION.md under unit testing for more details.

In the *player* module, we used the 0.0.0.0 to start the dealer and 0.0.0.0 and 8088 for player to connect to dealer's server. We tested various parameters to make sure the program outputted correct return values and error statements. The player.c is almost the same as training.c without updating Q and keeping track of states. By running training 50000 times we also test the message handling functionality of player. Both programs return 1 when received an invalid message.
 
In the *network* module, perform a unit test on the functionality using two test programs (one as a client, one as a server). Tests should:
1. Simulate game play using valid messages in the order they would appear (see REQUIREMENTS.md).
2. Test message-validation by attempting to send a series of erroneous messages, including server messages from the client, client messages from the server, invalid message structure (eg. CARD Jack Hearts), and typos.
 
 
In the *cards* module, a program *cardsTest* will serve as a unit test. It calls every function in the module and uses print statements to check the accuracy of the outputs. Tests should:
1. Create a new deck
2. Deal the cards in this deck out to two hands
3. Print each card and its value and print the value of both hands
4. Check Ace cases
5. Simulate card communication between dealer and player (turning cards to strings and strings to cards)
 
 
 
 

