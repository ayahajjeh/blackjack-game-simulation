/*
 * training.c - plays the game x times and updates Q table and saves it to a file.
 *              If training for the first time, there shouldn't be a Q table file.
 * 
 *
 * Barkin Cavdaroglu, Fall 2022
 * 
 */

// plays the game 10000 times and updates Q table and saves it to a file
#include "qtable.h"
#include <stdio.h>
#include "network.h"
#include "cards.h"
#include <stdlib.h>


card_t read_card(char *serverMessage, int counter) {
    char card[strlen(serverMessage) - counter];
    strncpy(card, serverMessage + counter + 1, strlen(serverMessage));
    card_t playerCard1 = string_toCard(card);
    return playerCard1;
}

int play_game_train(float ***Q, int ***Q_count, client_t *client, int round, int number_of_rounds) {
    char* serverMessage = readServerMessage(client);
    
    if (serverMessage != NULL) {
        // check if serverMessage is equal to BEGIN
        if (strcmp(serverMessage, "BEGIN") != 0) {
            free(serverMessage);
            return 1;
        }
        free(serverMessage);
        
    }

    hand_t* playerHand = hand_new();

    serverMessage = readServerMessage(client); // this should be card
    if (serverMessage != NULL) {
        // check if serverMessage is equal to CARD
        char *cardString = malloc(sizeof(char) * 4 + 1);
        strncpy(cardString, serverMessage, 4);
        if (strcmp(cardString, "CARD") != 0) {
            free(serverMessage);
            free(cardString);
            return 1;
        }
        free(cardString);
    }
    card_t playerCard1 = read_card(serverMessage, 4);
    hand_add(playerHand, playerCard1);
    free(serverMessage);

    serverMessage = readServerMessage(client); // this should be card
    if (serverMessage != NULL) {
        // check if serverMessage is equal to CARD
        char *cardString = malloc(sizeof(char) * 4 + 1);
        strncpy(cardString, serverMessage, 4);
        if (strcmp(cardString, "CARD") != 0) {
            free(serverMessage);
            free(cardString);
            return 1;
        }
        free(cardString);
    }
    card_t playerCard2 = read_card(serverMessage, 4);
    hand_add(playerHand, playerCard2);
    free(serverMessage);
    
    serverMessage = readServerMessage(client); // this should be card
    if (serverMessage != NULL) {
        // check if serverMessage is equal to CARD
        char *cardString = malloc(sizeof(char) * 6 + 1);
        strncpy(cardString, serverMessage, 6);
        if (strcmp(cardString, "DEALER") != 0) {
            free(serverMessage);
            free(cardString);
            return 1;
        }
        free(cardString);
    }
    card_t dealerCard = read_card(serverMessage, 6);
    int dealerCardValue = card_getValue(dealerCard);
    free(serverMessage);

    serverMessage = readServerMessage(client);
    if (serverMessage != NULL) {
        // check if serverMessage is equal to DECISION
        if (strcmp(serverMessage, "DECISION") != 0) {
            free(serverMessage);
            return 1;
        }
        free(serverMessage);
    }

    // choose whether to hit or stand randomly
    double hit = (double)rand() / (double)RAND_MAX;
    double stand = 1 - hit;
    
    int action;
    if (hit > stand) {
        action = 0;
    } else {
        action = 1;
    }

    float totalReward;

    int stateCount = 0;
    state_t *states = malloc(sizeof(state_t) * 50);
    state_t currState = {hand_getValue(playerHand), dealerCardValue, action};
    states[stateCount] = currState;
    stateCount++;

    if (action == 0) {
        sendToServer(client, "HIT");
    }
    else {
        sendToServer(client, "STAND");
    }
    
    // player tells the dealer their decision based on player’s cards and
    // dealer’s face-up card (repeat until STAND or BUST)
    while (action == 0) {
        serverMessage = readServerMessage(client); // this should be card
        if (serverMessage != NULL) {
            // check if serverMessage is equal to CARD
            char *cardString = malloc(sizeof(char) * 4 + 1);
            strncpy(cardString, serverMessage, 4);
            if (strcmp(cardString, "CARD") != 0) {
                free(serverMessage);
                free(cardString);
                return 1;
            }
            free(cardString);
        }
        card_t playerCard = read_card(serverMessage, 4);
        hand_add(playerHand, playerCard);
        free(serverMessage);

        if (hand_getValue(playerHand) > 21) {
            break;
        }

        serverMessage = readServerMessage(client);
        if (serverMessage != NULL) {
            // check if serverMessage is equal to DECISION
            if (strcmp(serverMessage, "DECISION") != 0) {
                free(serverMessage);
                return 1;
            }
            free(serverMessage);
        }

        double hit = (double)rand() / (double)RAND_MAX;
        double stand = 1 - hit;
        int action;
        if (hit > stand) {
            action = 0;
        } else {
            action = 1;
        }

        state_t currState = {hand_getValue(playerHand), dealerCardValue, action};
        states[stateCount] = currState;
        stateCount++;

        if (action == 0) {
            sendToServer(client, "HIT");
        }
        else {
            sendToServer(client, "STAND");
            break;
        }
    }

    serverMessage = readServerMessage(client);
    if (serverMessage != NULL) {
        // split serverMessage by spaces
        char *splitMessage = strtok(serverMessage, " ");
        // the first split should be equal to RESULT followed by WIN, LOSE, or PUSH
        // and the last message, if exists, should be BEGIN which restarts the game
        if (splitMessage != NULL) {
            if (strcmp(splitMessage, "RESULT") == 0) {
                splitMessage = strtok(NULL, " ");
                if (strcmp(splitMessage, "WIN") == 0) {
                    totalReward = 1;
                }
                else if (strcmp(splitMessage, "LOOSE") == 0) {
                    totalReward = -1;
                }
                else if (strcmp(splitMessage, "PUSH") == 0) {
                    totalReward = 0;
                }
                // now update Q table of all actions taken in this game with the reward
                for (int i = 0; i < stateCount; i++) {
                    state_t currState = states[i];

                    int handValue = hand_getValue(playerHand);
                    int dealerCardValue = currState.dealerCardValue;
                    int action = currState.action;
                    if (handValue < 22 && dealerCardValue < 22) {
                        update_Q(Q, Q_count, handValue, dealerCardValue, action, totalReward);
                    }
                }
            }
        }
    }

    return 0;
}

void training(const char *server_address, int PORT, int number_of_rounds) {
    int i;
    float ***Q = malloc(sizeof(float **) * 22);
    int ***Q_count = malloc(sizeof(int **) * 22);
    
    FILE *fp;
    fp = fopen("qtable.txt", "r");
    if (fp == NULL) {
        initialize_Q(Q, Q_count, 22, 22, 3);
    } else {
        load_Q(Q, 22, 22, 3);
    }

    client_t* client = startClient(server_address, PORT);
    sendToServer(client, "JOIN player1");

    for (i = 0; i < number_of_rounds; i++) {
        printf("Playing game %d\n", i);
        int status = play_game_train(Q, Q_count, client, i, number_of_rounds);
        printf("\n");
        if (status == 1) {
            save_Q(Q, 22, 22, 3);
            free_Q(Q, Q_count, 22, 22, 3);
            printf("Error in play_game\n");
            return;
        }   
    }
    closeClient(client);
    save_Q(Q, 22, 22, 3);
    free_Q(Q, Q_count, 22, 22, 3);
}

int entry_training(int argc, char* argv[]) {

    // run from the command line
    if (argc != 5) {
        printf("wrong number of parameters passed\n");
        exit(1);
    }

   // grab second arg
   char* server_address = argv[1];

   // ensure third arg is an int
   int PORT = 0;
   if(sscanf(argv[2], "%d", &PORT) != 1) {
      fprintf(stderr, "usage: ./player <server_address> <port>\n");
      exit(3);
   }

   int number_of_rounds = atoi(argv[3]);

   training(server_address, PORT, number_of_rounds);

   return 0;
}



