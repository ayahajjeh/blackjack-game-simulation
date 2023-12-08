/*
 * cards.c - module for CS50 blackjack final project
 *
 * see cards.h for more information
 *
 * Elaine Pu. 2022
 * CS 50, Fall 2022
 */

#include "cards.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void swap(card_t* a, card_t* b);

// a hand holds an array of at most 52 cards and a int to keep track of the
// "top" of the hand
typedef struct hand {
  card_t cards[52];
  int numCards;
} hand_t;

/**************** hand_new() ****************/
/* see cards.h for description */
hand_t* hand_new(void)
{
  hand_t* hand = malloc(sizeof(hand_t));
  hand->numCards = 0;
  return hand;
}

/**************** hand_delete() ****************/
/* see cards.h for description */
void hand_delete(hand_t* hand)
{
  if (hand != NULL) {
    free(hand);
  } else {
    fprintf(stderr, "Invalid hand\n");
  }
}

/**************** deck_new() ****************/
/* see cards.h for description */
deck_t* deck_new(void)
{
  deck_t* deck = hand_new();
  // set initial values for deck: 1-52 representing 52 cards
  for (int i = 0; i < 52; i++) {
    deck->cards[i] = i + 1;
  }
  deck->numCards = 52;
  return deck;
}

/**************** deck_delete() ****************/
/* see cards.h for description */
void deck_delete(deck_t* deck)
{
  if (deck != NULL) {
    hand_delete((hand_t*)deck);
  } else {
    fprintf(stderr, "Invalid deck\n");
  }
}

/**************** card_deal() ****************/
/* see cards.h for description */
card_t card_deal(deck_t* deck, hand_t* hand)
{
  if (deck != NULL && hand != NULL) {
    if (deck->numCards != 0) {
      // draw from the "top" of the deck
      card_t cardDealt = deck->cards[deck->numCards - 1];
      // add this card to hand
      hand->cards[hand->numCards] = cardDealt;
      // change pointers to the "top" of deck and hand
      deck->numCards--;
      hand->numCards++;
      return cardDealt;
    } else {
      fprintf(stdout, "Empty deck\n");
      return 0;
    }
  } else {
    fprintf(stderr, "Invalid deck or hand\n");
    return -1;
  }
}

/**************** deck_shuffle() ****************/
/* see cards.h for description */
deck_t* deck_shuffle(deck_t* deck)
{
  if (deck != NULL) {
    if (deck->numCards != 0) {
      srand(time(0));
      // loop through every index in deck
      for (int i = 0; i < deck->numCards; i++) {
        // pick a random index
        int indexRandom = 0;
        while (indexRandom == 0) {
          indexRandom = rand() % (deck->numCards - 1);
        }
        // swap the cards at the two indexes
        swap(&deck->cards[i], &deck->cards[indexRandom]);
      }
      return deck;
    } else {
      fprintf(stdout, "Empty deck\n");
      return deck;
    }
  } else {
    fprintf(stderr, "Invalid deck\n");
    return NULL;
  }
}

/**************** deck_isEmpty() ****************/
/* see cards.h for description */
bool deck_isEmpty(deck_t* deck)
{
  if (deck != NULL) {
    if (deck->numCards == 0) {
      return true;
    } else {
      return false;
    }
  } else {
    fprintf(stderr, "Invalid deck\n");
    return false;
  }
}

/**************** swap() ****************/
/* local function to swap two cards
 */
static void swap(card_t* a, card_t* b)
{
  card_t temp = *a;
  *a = *b;
  *b = temp;
}

/**************** card_getValue() ****************/
/* see cards.h for description */
int card_getValue(card_t card)
{
  // check if card is valid
  if (card > 0) {
    // if card is jack or queen
    if (((int)card) % 13 > 10) {
      return 10;
    }
    // if card is king
    else if (((int)card) % 13 == 0) {
      return 10;
    }
    // if card is ace
    else if (((int)card) % 13 == 1) {
      return 11;
    }
    // if card is 2-10
    else {
      return ((int)card) % 13;
    }
  } else {
    fprintf(stderr, "Invalid card\n");
    return -1;
  }
}

/**************** hand_numAces() ****************/
/* see cards.h for description */
int hand_numAces(hand_t* hand)
{
  // check if hand is valid
  if (hand != NULL) {
    int numAces = 0;
    // loop every card in hand
    for (int i = 0; i < hand->numCards; i++) {
      // if card is an ace
      if (card_getValue(hand->cards[i]) == 11) {
        numAces++;
      }
    }
    return numAces;
  } else {
    fprintf(stderr, "Invalid hand\n");
    return -1;
  }
}

/**************** hand_getValue() ****************/
/* see cards.h for description */
int hand_getValue(hand_t* hand)
{
  if (hand != NULL) {
    int sum = 0;
    // loop through every card in hand and add its value to sum
    for (int i = 0; i < hand->numCards; i++) {
      sum = sum + card_getValue(hand->cards[i]);
    }
    int numAces = hand_numAces(hand);
    // while sum is greater than 21 and there are Aces in the hand
    while (sum > 21 && numAces != 0) {
      sum = sum - 10;
      numAces--;
    }
    return sum;
  } else {
    fprintf(stderr, "Invalid hand\n");
    return -1;
  }
}

/**************** hand_add() ****************/
/* see cards.h for description */
hand_t* hand_add(hand_t* hand, card_t card)
{
  // check if hand and card are valid
  if (hand != NULL) {
    if (card > 0) {
      // add card to “top” of hand
      hand->cards[hand->numCards] = card;
      // increment pointer to “top” of hand
      hand->numCards++;
      return hand;
    } else {
      fprintf(stderr, "invalid card\n");
      return NULL;
    }
  } else {
    fprintf(stderr, "invalid hand\n");
    return NULL;
  }
}

/**************** card_toString() ****************/
/* see cards.h for description */
char* card_toString(card_t card)
{
  // check if card is valid
  if (card > 0) {
    char* cardName = malloc(sizeof(char) * 20);
    *cardName = 0;
    char* filler = " of ";
    // match the int value of card to its corresponding value in string
    if (((int)card) % 13 == 1) {
      char* cardValue = "Ace";
      strncat(cardName, cardValue, strlen(cardValue));
    } else if (((int)card) % 13 == 2) {
      char* cardValue = "Two";
      strncat(cardName, cardValue, strlen(cardValue));
    } else if (((int)card) % 13 == 3) {
      char* cardValue = "Three";
      strncat(cardName, cardValue, strlen(cardValue));
    } else if (((int)card) % 13 == 4) {
      char* cardValue = "Four";
      strncat(cardName, cardValue, strlen(cardValue));
    } else if (((int)card) % 13 == 5) {
      char* cardValue = "Five";
      strncat(cardName, cardValue, strlen(cardValue));
    } else if (((int)card) % 13 == 6) {
      char* cardValue = "Six";
      strncat(cardName, cardValue, strlen(cardValue));
    } else if (((int)card) % 13 == 7) {
      char* cardValue = "Seven";
      strncat(cardName, cardValue, strlen(cardValue));
    } else if (((int)card) % 13 == 8) {
      char* cardValue = "Eight";
      strncat(cardName, cardValue, strlen(cardValue));
    } else if (((int)card) % 13 == 9) {
      char* cardValue = "Nine";
      strncat(cardName, cardValue, strlen(cardValue));
    } else if (((int)card) % 13 == 10) {
      char* cardValue = "Ten";
      strncat(cardName, cardValue, strlen(cardValue));
    } else if (((int)card) % 13 == 11) {
      char* cardValue = "Jack";
      strncat(cardName, cardValue, strlen(cardValue));
    } else if (((int)card) % 13 == 12) {
      char* cardValue = "Queen";
      strncat(cardName, cardValue, strlen(cardValue));
    } else if (((int)card) % 13 == 0) {
      char* cardValue = "King";
      strncat(cardName, cardValue, strlen(cardValue));
    }
    // Concatenate filler to cardName
    strncat(cardName, filler, strlen(filler));
    // match the int value of card to its corresponding suit in string
    if (((int)card) >= 1 && ((int)card) <= 13) {
      char* cardSuit = "Clubs";
      strncat(cardName, cardSuit, strlen(cardSuit));
    } else if (((int)card) >= 14 && ((int)card) <= 26) {
      char* cardSuit = "Diamonds";
      strncat(cardName, cardSuit, strlen(cardSuit));
    } else if (((int)card) >= 27 && ((int)card) <= 39) {
      char* cardSuit = "Hearts";
      strncat(cardName, cardSuit, strlen(cardSuit));
    } else if (((int)card) >= 40 && ((int)card) <= 52) {
      char* cardSuit = "Spades";
      strncat(cardName, cardSuit, strlen(cardSuit));
    }
    return cardName;

  } else {
    fprintf(stderr, "Invalid card\n");
    return NULL;
  }
}

/**************** string_toCard() ****************/
/* see cards.h for description */
card_t string_toCard(char* cardName)
{
  // record length of string
  int length = strlen(cardName);
  char* words[3];
  int index = 0;
  bool isSpaceBefore = true;
  while (index < 3) {
    // loop all characters in cardName
    for (int i = 0; i < length; i++) {
      // if character is a letter and there is a space before
      if (isalpha(cardName[i]) && isSpaceBefore) {
        words[index] = cardName + i;
        index++;
        isSpaceBefore = false;
      }
      // if character is a space
      else if (isspace(cardName[i])) {
        cardName[i] = '\0';
        isSpaceBefore = true;
      }
    }
  }
  // turn words into lower case
  for (int i = 0; i < index; i++) {
    for (int n = 0; n < strlen(words[i]); n++) {
      words[i][n] = (char)tolower(words[i][n]);
    }
  }
  // interpret the first and third word
  int multiplier = 0;
  if (strcmp(words[2], "clubs") == 0) {
    multiplier = 0;
  } else if (strcmp(words[2], "diamonds") == 0) {
    multiplier = 1;
  } else if (strcmp(words[2], "hearts") == 0) {
    multiplier = 2;
  } else if (strcmp(words[2], "spades") == 0) {
    multiplier = 3;
  } else {
    fprintf(stderr, "Invalid cardName\n");
    return -1;
  }
  int add = 0;
  // compare the first word with ace-king and record the corresponding face
  // values
  if (strcmp(words[0], "ace") == 0) {
    add = 1;
  } else if (strcmp(words[0], "two") == 0) {
    add = 2;
  } else if (strcmp(words[0], "three") == 0) {
    add = 3;
  } else if (strcmp(words[0], "four") == 0) {
    add = 4;
  } else if (strcmp(words[0], "five") == 0) {
    add = 5;
  } else if (strcmp(words[0], "six") == 0) {
    add = 6;
  } else if (strcmp(words[0], "seven") == 0) {
    add = 7;
  } else if (strcmp(words[0], "eight") == 0) {
    add = 8;
  } else if (strcmp(words[0], "nine") == 0) {
    add = 9;
  } else if (strcmp(words[0], "ten") == 0) {
    add = 10;
  } else if (strcmp(words[0], "jack") == 0) {
    add = 11;
  } else if (strcmp(words[0], "queen") == 0) {
    add = 12;
  } else if (strcmp(words[0], "king") == 0) {
    add = 13;
  } else {
    fprintf(stderr, "Invalid cardName\n");
    return -1;
  }
  return (card_t)(13 * multiplier + add);
}

/**************** hand_print() ****************/
/* see cards.h for description */
void hand_print(hand_t* hand, FILE* fp)
{
  // if file and hand are valid
  if (fp != NULL) {
    if (hand != NULL) {
      if (hand->numCards != 0) {
        // loop through every card in hand and print its value in the cards
        // array
        for (int i = 0; i < hand->numCards; i++) {
          fprintf(fp, "[%d] ", hand->cards[i]);
        }
        fputs("\n", fp);
      } else {
        fputs("(empty)\n", fp);
      }
    } else {
      fprintf(stderr, "Invalid hand\n");
    }
  } else {
    fprintf(stderr, "Unable to open file\n");
  }
}

/**************** deck_print() ****************/
void deck_print(deck_t* deck, FILE* fp) { hand_print((hand_t*)deck, fp); }