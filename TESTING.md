# Testing: Team 8
 
### Functionality test for dealer and player:
Functionality test output:
* dealer and player run on a server-client relationship, so unit testing is not possible (without effectively writing a dealer/player simulator). However, we unit-tested the dealer module by checking and validating the parameters passed. The dealer module was tested by `testing.sh`. It passes one argument and then passes three arguments. It also passes a non-integer port number and a non-integer number of rounds.
* In the *dealer* module, we used the *nc 0.0.0.0 [portNumber]* command to connect to the port the dealer was listening to. After connecting to the same socket, we were testing the dealer by sending messages from the player-simulating terminal like *JOIN player1* and *HIT*, and *STAND*. We tested edge cases to make sure the dealer hands them well. However, whenever an invalid message is sent to the dealer, the dealer exits non-zero. We also provided a testing.sh file to test arguments passed to the dealer. Check IMPLEMENTATION.md under unit testing for more details.
* In the *player* module, we used the 0.0.0.0 to start the dealer and 0.0.0.0 and 8088 for player to connect to dealer's server. We tested various parameters to make sure the program outputted correct return values and error statements. The player.c is almost the same as training.c without updating Q and keeping track of states. By running training 50000 times we also test the message handling functionality of player. Both programs return 1 when received an invalid message.
 
 
### Functionality test for network:
List of test cases:
* Test 1: Simulate game play using valid messages in the order they would appear (see REQUIREMENTS.md).
* Test 2: Test message-validation by attempting to send a series of erroneous messages, including server messages from the client, client messages from the server, invalid message structure (eg. CARD Jack Hearts), and typos.
 
Functionality test output: 
* navigate to ‘network’ directory
* start serverTest using: `./serverTest <PORT>` 
* start clientTest using: `./clientTest <server_addr> <PORT>`
* Note: `wget -O - -q https://checkip.amazonaws.com` allows you to access IP Address
 
Memory test:
* tested with myvalgrind to ensure there we no memory leaks
 
### Functionality test for cards:
List of test cases:
* TEST 1: Create a new deck and shuffle it
* TEST 2: Deal the cards in this deck out to two hands
* TEST 3: Print each card and its value and print the value of both hands
* TEST 4: Check Ace cases
* TEST 5: Simulate card communication between dealer and player (turning cards to strings and strings to cards)
 
Functionality test output: 
* navigate to ‘cards’ directory
* use *make test* to run test and output to *testing.out* 
* expected output for each test is printed along with the output
 
Memory test:
* use *make valgrind* to run memory test
* expected output: no memory leaks and no errors

