# CS50 Final Project: 21 (Blackjack)
## Team 8: Barkin Cavdaroglu, Aya Hajjeh, Daniel Lampert, Elaine Pu
 
## Top-level README file
This file gives an overview of the program, including any assumptions made while completing the project. For specifications, see REQUIREMENTS.md, DESIGN.md, and IMPLEMENTATION.md
 
### the program
The program functions on two main components: the dealer and the player. Through TCP/IP communication, the dealer runs a game of 21 for the player, which uses reinforcement learning over a series of training sessions to develop an optimized strategy to play the game given different game states.
 
### Assumptions
We assume that the player knows the syntax of message exchanging between the player and the dealer. If any sent message is not valid, the program just exits non-zero and does not ask the player/the user to re-enter their message. The player/user will have to re-run the program.
 
### Limitations
The Network module uses a delay when sending of 50 ms to prevent overlapping messages. This value was found experimentally.
 
### Files
Dealer:
* `Makefile` - compilation procedure
* `dealer.c` - dealer implementation
* `testing.sh - short testing script
* `testing.out` - testing results
 
Player:
* `Makefile` - compilation procedure
* `training.h` - training mode header with function declarations
* `training.c` - the implementation
* `player.h` - player mode header header with function declarations
* `player.c` - the implementation
* `entry_point.c` - program that handles choosing modes
* `qtable.h` - q-table header with function declarations
* `qtable.c` - the implementation
* `qtable.txt` - training table written to disk
 
 
Cards:
* `Makefile` - compilation procedure
* `cards.h` - the header with function declarations
* `cards.c` - the implementation
* `cardTest.c` - unit test driver
 
Network:
 
* `Makefile` - compilation procedure
* `network.h` - network module header file
* `network.c` - the implementation
* `serverTest.c` - server-specific unit test
* `clientTest.c` - client-specific unit test
 
 
### Compilation
 
To compile, simply `make all`.
 
### Testing
Testing is covered in TESTING.md
 


