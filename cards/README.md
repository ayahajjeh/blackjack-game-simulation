# CS50 Final Project 

### Cards

The *cards* module holds a card struct, hand struct, and deck struct. It supports the creation and removal of the deck struct and hand struct as well as functions that will be called by the dealer or player to perform some action on the cards or return some information about the cards. 

### Usage

The *cards* module, defined in `cards.h` and implemented in `cards.c`, handles functionalities with regards to the playing cards used in the game. We choose to create this module separately for ease of testing and anticipating that it will be used by the dealer and player.


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

### Implementation

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

### Assumptions

No assumptions beyond those that are clear from the spec.

### Files

* `Makefile` - compilation procedure
* `cards.h` - the header with function declarations
* `cards.c` - the implementation
* `cardTest.c` - unit test driver

### Compilation

To compile, simply `make`.

### Testing

The `cardsTest.c` program tests calls every function in the module and uses print statements to check the accuracy of the outputs. Tests should:
1. Create a new deck
2. Deal the cards in this deck out to two hands
3. Print each card and its value and print the value of both hands
4. Check Ace cases
5. Simulate card communication between dealer and player (turning cards to strings and strings to cards)

To test, simply `make test`.
See `testing.out` for details of testing and an example test run.

To test with valgrind, `make valgrind`.
