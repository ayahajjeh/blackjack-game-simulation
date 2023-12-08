/*
 *
 * dealer.c is in charge of running the blackjack game.
 * The dealer deals cards to the player and to themselves, and then waits for decisions from the player until
 * the player STANDs. Then, the dealer calculates the result of the game and sends it back to the player.
 * The dealer restarts the game every time we start a new round.
 * The dealer repeats playing for the same number of rounds entered by the user.
 * 
 * usage: ./dealer portNumber numberOfRounds
 *
 *
 * Aya Hajjeh, Team 8, Nov 4, 2022
 * Final Project
 * CS50, Fall22
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "../network/network.h"
#include "../cards/cards.h"

/**************** local functions prototypes ****************/
void printUsage(FILE* fp);
char* calculateGameResult(hand_t* playerHand, hand_t* dealerHand);

/**************** main() ****************/
int main(int argc, char* argv[]){

    if (argc != 3) {
        fprintf(stderr, "wrong number of parameters passed\n");
        fprintf(stderr, "usage is: ");
        printUsage(stderr);
        exit(1);
    }

    int port;
    if (sscanf(argv[1], "%d", &port) != 1) {
      fprintf(stderr, "port must be an integer value\n");
      exit(2);
    }

    int numberOfRounds;
   if (sscanf(argv[2], "%d", &numberOfRounds) != 1) {
      fprintf(stderr, "numberOfRounds must be an integer value\n");
      exit(3);
    }

    // set up a server socket listening for clients (players) to connect (see this server example)
    server_t* server = startServer(port);

    // if server does not start, exit code 4
    if (server == NULL) {
        fprintf(stderr, "unable to establish server\n");
        exit(4);
    }

    // read for "JOIN <playername>" message
    char* clientMessage = readClientMessage(server);
    if (clientMessage != NULL) {
        free(clientMessage);
    }

    deck_t* deck;
    hand_t* playerHand;
    hand_t* dealerHand;

    for (int i=0; i<numberOfRounds; i++){
    
    // send "BEGIN" message
    if (!sendToClient(server, "BEGIN")) {
        fprintf(stderr, "error sending messsage to client\n");
        closeServer(server);
        exit(5);
    }
    
    // pass messages back and forth over socket as shown in diagram below, and
        playerHand = hand_new();
        dealerHand = hand_new();
        card_t card;

        // create a deck of 52 cards for each game
        deck = deck_new();

        // shuffle the deck
        deck = deck_shuffle(deck);

        // deal cards to the player by sending messages with the card suit and rank as a string (e.g., “Seven of Hearts”)
        // deal first card
        card = card_deal(deck, playerHand);
        char* cardString = card_toString(card);
        char* message = malloc(strlen("CARD ") + strlen(cardString) + 1);
        strcpy(message, "CARD ");
        strcat(message, cardString);
        if (!(sendToClient(server, message))){
            fprintf(stderr, "could not send message to the client of what the first card of the player is\n");
            if (cardString != NULL)
                free(cardString);
            if (message != NULL)
                free(message);
            if (playerHand != NULL)
                hand_delete(playerHand);
            if (dealerHand != NULL)
                hand_delete(dealerHand);
            if (deck != NULL)
                deck_delete(deck);
            closeServer(server);
            exit(6);
        }
        if (cardString != NULL)
            free(cardString);
        if (message != NULL)
            free(message);
        
        // deal second card
        card = card_deal(deck, playerHand);
        cardString = card_toString(card);
        message = malloc(strlen("CARD ") + strlen(cardString) + 1);
        strcpy(message, "CARD ");
        strcat(message, cardString);
        if (!(sendToClient(server, message))){
            fprintf(stderr, "could not send message to the client of what the second card of the player is\n");
            if (cardString != NULL)
                free(cardString);
            if (message != NULL)
                free(message);
            if (playerHand != NULL)
                hand_delete(playerHand);
            if (dealerHand != NULL)
                hand_delete(dealerHand);
            if (deck != NULL)
                deck_delete(deck);
            closeServer(server);
            exit(7);
        }
        if (cardString != NULL)
            free(cardString);
        if (message != NULL)
            free(message);

        // deal third card
        card = card_deal(deck, dealerHand);
        cardString = card_toString(card);
        message = malloc(strlen("DEALER ") + strlen(cardString) + 1);
        strcpy(message, "DEALER ");
        strcat(message, cardString);
        if (!(sendToClient(server, message))){
            fprintf(stderr, "could not send message to the client of what the first card of the dealer is\n");
            if (cardString != NULL)
                free(cardString);
            if (message != NULL)
                free(message);
            if (playerHand != NULL)
                hand_delete(playerHand);
            if (dealerHand != NULL)
                hand_delete(dealerHand);
            if (deck != NULL)
                deck_delete(deck);
            closeServer(server);
            exit(8);
        }
        if (cardString != NULL)
            free(cardString);
        if (message != NULL)
            free(message);
        
        // deal fourth card
        card_deal(deck, dealerHand);
        message = "DECISION";
        if (!(sendToClient(server, message))){
            fprintf(stderr, "could not send `DECISION` message to client\n");
            if (playerHand != NULL)
                hand_delete(playerHand);
            if (dealerHand != NULL)
                hand_delete(dealerHand);
            if (deck != NULL)
                deck_delete(deck);
            closeServer(server);
            exit(9);
        }

        // receive HIT/STAND decisions from the client
        char* playerDecision = readClientMessage(server);
        while (strcmp("STAND", playerDecision) != 0){
            if (strcmp("HIT", playerDecision) == 0){
                // send the card string to client
                if (deck == NULL){
                    fprintf(stderr, "error empty deck\n");
                    if (playerHand != NULL)
                        hand_delete(playerHand);
                    if (dealerHand != NULL)
                        hand_delete(dealerHand);
                    if (playerDecision != NULL)
                        free(playerDecision);
                    closeServer(server);
                    exit(10);
                }
                card = card_deal(deck, playerHand);
                cardString = card_toString(card);
                message = malloc(strlen("CARD ") + strlen(cardString) + 1);
                strcpy(message, "CARD ");
                strcat(message, cardString);
                if (!(sendToClient(server, message))){
                    fprintf(stderr, "could not send message to the client of what the card of the player is after they hit\n");
                    if (cardString != NULL)
                        free(cardString);
                    if (message != NULL)
                        free(message);
                    if (playerHand != NULL)
                        hand_delete(playerHand);
                    if (dealerHand != NULL)
                        hand_delete(dealerHand);
                    if (deck != NULL)
                        deck_delete(deck);
                    if (playerDecision != NULL)
                        free(playerDecision);
                    closeServer(server);
                    exit(11);
                }
                if (cardString != NULL)
                    free(cardString);
                if (message != NULL)
                    free(message);
                
    
                //check if the player bust
                if (hand_getValue(playerHand) > 21)
                    break;
                message = "DECISION";
                if (!sendToClient(server, message)){
                    fprintf(stderr, "could not send `DECISION` message to client\n");
                    if (deck != NULL)
                        deck_delete(deck);
                    if (playerHand != NULL)
                        hand_delete(playerHand);
                    if (dealerHand != NULL)
                        hand_delete(dealerHand);
                    if (playerDecision != NULL)
                        free(playerDecision);
                    closeServer(server);
                    exit(12);
                }
                if (playerDecision != NULL)
                    free(playerDecision);
                playerDecision = readClientMessage(server);
            }
            else {
                fprintf(stderr, "received `%s` an invalid message from the player that is neither HIT nor STAND\n", playerDecision);
                if (deck != NULL)
                        deck_delete(deck);
                if (playerHand != NULL)
                        hand_delete(playerHand);
                if (dealerHand != NULL)
                        hand_delete(dealerHand);
                if (playerDecision != NULL)
                    free(playerDecision);
                closeServer(server);
                exit(13);
            }
        }
        if (playerDecision != NULL)
            free(playerDecision);

        //calculate the dealer hand value and decide whether to HIT or STAND
        // since the dealer minimum value is 17
        int* dealerHandValue = malloc(sizeof(int));
        (*dealerHandValue) = hand_getValue(dealerHand);
        const int dealerHandMinimumValue = 17;
        if (hand_getValue(playerHand) <= 21){
            while ((*dealerHandValue) < dealerHandMinimumValue){
                card_t card = card_deal(deck, dealerHand);
                (*dealerHandValue) = hand_getValue(dealerHand);
                if (card <= 0){
                    fprintf(stderr, "could not deal card to the dealer when they were below 17\n");
                    if (playerHand != NULL)
                        hand_delete(playerHand);
                    if (dealerHand != NULL)
                        hand_delete(dealerHand);
                    if (deck != NULL)
                        deck_delete(deck);
                    if (dealerHandValue != NULL)
                        free(dealerHandValue);
                    closeServer(server);
                    exit(14);
                }
            }
        }

        // calculate the game’s result (WIN, LOOSE, or PUSH) and send a message to the client
        char* result = calculateGameResult(playerHand, dealerHand);
        if (result == NULL){
            if (deck != NULL)
                deck_delete(deck);
            if (playerHand != NULL)
                hand_delete(playerHand);
            if (dealerHand != NULL)
                hand_delete(dealerHand);
            if (dealerHandValue != NULL)
                free(dealerHandValue);
            closeServer(server);
            exit(15);
        }
        sendToClient(server, result);

        // reset and play again (you decide on how many games to play)
        if (deck != NULL)
            deck_delete(deck);
        if (playerHand != NULL)
            hand_delete(playerHand);
        if (dealerHand != NULL)
            hand_delete(dealerHand);
        if (dealerHandValue != NULL)
            free(dealerHandValue);
    }

    // send a QUIT message to the client when done
    char* message = "QUIT";
    sendToClient(server, message);
    closeServer(server);
    return 0;
}

/**
 * purpose: prints out the usage of the dealer in a command line
 * parameters: file pointer to the file where we want to print out the usage
 * return: none
 * */
/**************** printUsage() ****************/
void printUsage(FILE* fp){
    fprintf(fp, "./dealer portNumber numberOfRounds\n");
}

/**
 * purpose: calculates the result of a one round blackjack game
 * parameters: a player hand and a dealer hand
 * return: a string of the result of the game.
 * It returns NULL if any of the parameters are invalid or if none of the possible results apply
 * */
/**************** calculateGameResult() ****************/
char* calculateGameResult(hand_t* playerHand, hand_t* dealerHand){
    if (playerHand == NULL){
        fprintf(stderr, "RESULT player hand is NULL");
        return NULL;
    }
    int playerHandValue = hand_getValue(playerHand);
    if (dealerHand == NULL){
        fprintf(stderr, "RESULT dealer hand is NULL");
        return NULL;
    }
    int dealerHandValue = hand_getValue(dealerHand);
    if (playerHandValue > 21)
        return "RESULT LOOSE";
    if (dealerHandValue > 21)
        return "RESULT WIN";
    if (playerHandValue == dealerHandValue)
        return "RESULT PUSH";
    if (playerHandValue > dealerHandValue)
        return "RESULT WIN";
    if (playerHandValue < dealerHandValue)
        return "RESULT LOOSE";
    fprintf(stderr, "invalid game result calculated");
    return NULL;
}