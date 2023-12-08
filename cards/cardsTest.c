/*
 * cardsTest.c - test for cards module
 *
 * test various functions of cards module
 *
 * Elaine Pu. 2022
 * CS 50, Fall 2022
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cards.h"

int main(const int argc, char* argv[])
{
  printf("\n-----create new deck-----\n");
  // initialize new deck
  deck_t* deck = deck_new();
  printf("should contain values 1-52 in order:\ndeck: ");
  // print cards array in deck

  deck_print(deck, stdout);

  printf("\n-----shuffle new deck-----\n");
  // shuffle deck
  deck = deck_shuffle(deck);
  printf("should contain values 1-52 in random order:\ndeck: ");
  // print cards array in deck
  deck_print(deck, stdout);

  // deck_t* deck2 = deck_new();
  //  deck2 = deck_shuffle(deck2);
  //  deck_print(deck2, stdout);

  // create two empty hands
  printf("\n-----create new hand A-----\n");
  hand_t* handA = hand_new();
  printf("should be empty:\nhand: ");
  hand_print(handA, stdout);

  printf("\n-----create new hand B-----\n");
  hand_t* handB = hand_new();
  printf("should be empty:\nhand: ");
  hand_print(handB, stdout);

  printf("\n-----deal 10 cards to hand A-----\n");
  // deal 10 cards to hand A
  for (int i = 0; i < 10; i++) {
    card_t card = card_deal(deck, handA);
    // print each card
    char* cardString = card_toString(card);
    printf("%s ", cardString);
    free(cardString);
    // print its value
    printf("(card value: %d)\n", card_getValue(card));
  }
  printf("should have 10 cards:\nhand: ");
  // print cards array
  hand_print(handA, stdout);
  //print number of Aces
  printf("number of Aces: %d\n", hand_numAces(handA));
  // print total value of cards in hand A
  printf("total value: %d\n", hand_getValue(handA));

  printf("\n-----deal 10 cards to hand B-----\n");
  // deal 10 cards to hand B
  for (int i = 0; i < 10; i++) {
    card_t card = card_deal(deck, handB);
    // print each card
    char* cardString = card_toString(card);
    printf("%s ", cardString);
    free(cardString);
    // print its value
    printf("(card value: %d)\n", card_getValue(card));
  }
  printf("should have 10 cards:\nhand: ");
  // print cards array in hand B
  hand_print(handB, stdout);
  //print number of Aces
  printf("number of Aces: %d\n", hand_numAces(handB));
  // print total value of cards in hand B
  printf("total value: %d\n", hand_getValue(handB));

  printf("\n-----check deck-----\n");
  // check if deck is empty
  if (deck_isEmpty(deck)) {
    printf("empty deck\n");
  } else {
    printf("deck not empty\n");
  }
  printf("should have 32 cards:\ndeck: ");
  // print cards array in deck
  deck_print(deck, stdout);

  printf("\n-----shuffle again and deal out all cards-----\n");
  // shuffle deck
  deck = deck_shuffle(deck);
  // print cards array in deck
  printf("should have 32 cards in another order:\ndeck: ");
  deck_print(deck, stdout);
  card_t card;
  // deal out all cards in deck
  while ((card = card_deal(deck, handA)) != 0) {
    // print each card and its value
    char* cardString = card_toString(card);
    printf("%s ", cardString);
    free(cardString);
    printf("(card value: %d)\n", card_getValue(card));
  }

  printf("\n-----check deck_isEmpty function-----\n");
  // check if deck is empty
  printf("should be empty:\ndeck: ");
  if (deck_isEmpty(deck)) {
    printf("empty deck\n");
  } else {
    printf("deck not empty\n");
  }

  printf("\n-----simulate player-----\n");

  printf("---create hand---\n");
  // create empty hand
  hand_t* handC = hand_new();

  printf("should be empty:\nhand: ");
  hand_print(handC, stdout);

  printf("---pass card as a string---\n");
  // turn a string into a card
  char cardName[] = "Ace of Hearts";
  card_t newCard = string_toCard(cardName);
  char* cardString = card_toString(newCard);
  printf("card: %s (should be Ace of Hearts)\n", cardString);
  free(cardString);

  printf("---add card to hand---\n");
  //add card to hand
  handC = hand_add(handC, newCard);
  printf("hand: ");
  hand_print(handC, stdout);

  printf("---check Aces---\n");
  //check current value of hand C
  printf("current value of hand: %d (should be 11)\n", hand_getValue(handC));
  //add card to handC
  char cardName2[] = "Ace of Spades";
  card_t newCard2 = string_toCard(cardName2);
  handC = hand_add(handC, newCard2);
  //check value of hand
  printf("current value of hand: %d (should be 12)\n", hand_getValue(handC));
  //add another card to handC
  char cardName3[] = "Ace of Diamonds";
  card_t newCard3 = string_toCard(cardName3);
  handC = hand_add(handC, newCard3);
  //check value of hand
  printf("current value of hand: %d (should be 13)\n", hand_getValue(handC));


  // free decks, hands
  deck_delete(deck);
  //deck_delete(deck2);
  hand_delete(handA);
  hand_delete(handB);
  hand_delete(handC);

  return 0;
}