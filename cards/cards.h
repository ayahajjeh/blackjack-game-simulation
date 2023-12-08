/* 
 * cards.h - header file for cards module for CS50 blackjack final project
 *
 * Creates a deck and keeps track of hands and cards
 *
 * Elaine Pu, 2022
 * CS 50, Fall 2022
 */

#ifndef __CARDS_H
#define __CARDS_H

#include <stdbool.h>
#include <stdio.h>

/**************** global types ****************/
typedef int card_t;
typedef struct hand hand_t;
typedef hand_t deck_t;

/**************** functions ****************/

/**************** deck_new ****************/
/* Create a new card deck of 52 cards.
 *
 * We return:
 *   pointer to a new deck
 *
 * Caller is responsible for:
 *   later calling deck_delete.
 */
deck_t* deck_new(void);

/**************** deck_delete ****************/
/* Deletes a deck.
 */
void deck_delete(deck_t* deck);

/**************** hand_new ****************/
/* Create a new (empty) hand.
 *
 * We return:
 *   pointer to a new hand
 *
 * Caller is responsible for:
 *   later calling hand_delete.
 */
hand_t* hand_new(void);

/**************** hand_delete ****************/
/* Deletes a hand.
 */
void hand_delete(hand_t* hand);

/**************** card_deal ****************/
/* adds a card from the deck to the given hand
 *
 * We return:
 *   A card, or 0 if empty deck, -1 if error.
 *
 * Caller is responsible for:
 *   calling deck_shuffle at the start of the game before dealing the card.
 *
 */
card_t card_deal(deck_t* deck, hand_t* hand);

/**************** deck_shuffle ****************/
/* shuffles the deck.
 *
 * We return:
 *   pointer to a shuffled deck, or NULL if error.
 *
 */
deck_t* deck_shuffle(deck_t* deck);

/**************** deck_isEmpty ****************/
/* checks if deck is empty
 *
 * We return:
 *   true if deck is empty, false if not
 *
 */
bool deck_isEmpty(deck_t* deck);

/**************** card_getValue ****************/
/* gets a card's value
 *
 * We return:
 *   card's value as an integer, or -1 if error
 *
 */
int card_getValue(card_t card);

/**************** hand_numAces ****************/
/* get number of Aces in a hand
 *
 * We return:
 *   number of Aces in a hand, or -1 if error
 *
 */
int hand_numAces(hand_t* hand);

/**************** hand_getValue ****************/
/* gets a hand's total value
 *
 * We return:
 *   hand's value as an integer, or -1 if error
 *
 */
int hand_getValue(hand_t* hand);

/**************** hand_add ****************/
/* adds a card to hand
 *
 */
hand_t* hand_add(hand_t* hand, card_t card);

/**************** card_toString ****************/
/* returns a string holding the card and its suit
 *
 * We return:
 *   card as a string, or NULL if error
 * 
 * Caller is responsible for:
 *   deleting the string after use
 * 
 */
char* card_toString(card_t card);

/**************** string_toCard ****************/
/* returns a card from a given string
 *
 * We return:
 *   a card representing the given string, or -1 if error
 */
card_t string_toCard(char* name);

/**************** hand_print ****************/
/* prints all values in hand to file provided
 * for testing cards
 */
void hand_print(hand_t* hand, FILE* fp);

/**************** deck_print ****************/
/* prints all values in deck to file provided
 * for testing cards
 */
void deck_print(deck_t* deck, FILE* fp);

#endif  // __CARDS_H
