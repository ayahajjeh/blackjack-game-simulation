/*
 * player.c - plays the game by reading the Q table and sending messages to the server.
 *            Expects "qtable.txt" file to be in the same directory.
 *
 *
 * Barkin Cavdaroglu, Fall 2022
 *
 */
#include "qtable.h"
#include <stdio.h>
#include "network.h"
#include "cards.h"
#include <stdlib.h>
#include "training.h"

int play_game(float ***Q, client_t *client)
{
    hand_t *playerHand = hand_new();

    char *serverMessage = readServerMessage(client); // this should be card
    if (serverMessage != NULL)
    {
        // check if serverMessage is equal to CARD
        char *buffer = malloc(strlen(serverMessage) + 1);
        if (sscanf(serverMessage, "CARD %s", buffer) != 1)
        {
            free(serverMessage);
            printf("Error: Server message is not CARD");
            return 1;
        }
        free(buffer);
    }
    card_t playerCard1 = read_card(serverMessage, 4);
    hand_add(playerHand, playerCard1);
    free(serverMessage);

    serverMessage = readServerMessage(client); // this should be card
    if (serverMessage != NULL)
    {
        // check if serverMessage is equal to CARD
        char *buffer = malloc(strlen(serverMessage) + 1);
        if (sscanf(serverMessage, "CARD %s", buffer) != 1)
        {
            free(serverMessage);
            printf("Error: Server message is not CARD");
            return 1;
        }
        free(buffer);
    }
    card_t playerCard2 = read_card(serverMessage, 4);
    hand_add(playerHand, playerCard2);
    free(serverMessage);

    serverMessage = readServerMessage(client); // this should be card
    if (serverMessage != NULL)
    {
        // check if serverMessage is equal to CARD
        char *buffer = malloc(strlen(serverMessage) + 1);
        if (sscanf(serverMessage, "DEALER %s", buffer) != 1)
        {
            free(serverMessage);
            printf("Error: Server message is not DEALER");
            return 1;
        }
        free(buffer);
    }
    card_t dealerCard = read_card(serverMessage, 6);
    int dealerCardValue = card_getValue(dealerCard);
    free(serverMessage);

    serverMessage = readServerMessage(client);
    if (serverMessage != NULL)
    {
        // check if serverMessage is equal to DECISION
        if (strcmp(serverMessage, "DECISION") != 0)
        {
            printf("Error: Server message is not DECISION");
            return 1;
        }
        free(serverMessage);
    }

    // choose whether to hit or stand randomly
    double hit = Q[hand_getValue(playerHand)][dealerCardValue][0];
    double stand = Q[hand_getValue(playerHand)][dealerCardValue][1];

    if (hit == 0.0 && stand == 0.0)
    {
        hit = (double)rand() / (double)RAND_MAX;
        stand = 1 - hit;
    }

    int action;
    if (hit > stand)
    {
        action = 0;
    }
    else
    {
        action = 1;
    }

    if (action == 0)
    {
        if (!sendToServer(client, "HIT"))
        {
            printf("Error sending HIT to server\n");
            return 1;
        }
    }
    else
    {
        if (!sendToServer(client, "STAND"))
        {
            printf("Error sending STAND to server\n");
            return 1;
        }
    }

    // player tells the dealer their decision based on player’s cards and
    // dealer’s face-up card (repeat until STAND or BUST)
    while (action == 0)
    {
        serverMessage = readServerMessage(client); // this should be card
        if (serverMessage != NULL)
        {
            // check if serverMessage is equal to CARD
            char *buffer = malloc(strlen(serverMessage) + 1);
            if (sscanf(serverMessage, "CARD %s", buffer) != 1)
            {
                free(serverMessage);
                printf("Error: Server message is not CARD");
                return 1;
            }
            free(buffer);
        }
        card_t playerCard = read_card(serverMessage, 4);
        hand_add(playerHand, playerCard);
        free(serverMessage);

        if (hand_getValue(playerHand) > 21)
        {
            break;
        }

        serverMessage = readServerMessage(client);
        if (serverMessage != NULL)
        {
            // check if serverMessage is equal to DECISION
            if (strcmp(serverMessage, "DECISION") != 0)
            {
                printf("Error: Server message is not DECISION");
                return 1;
            }
            free(serverMessage);
        }

        double hit = Q[hand_getValue(playerHand)][dealerCardValue][0];
        double stand = Q[hand_getValue(playerHand)][dealerCardValue][1];

        if (hit == 0.0 && stand == 0.0)
        {
            hit = (double)rand() / (double)RAND_MAX;
            stand = 1 - hit;
        }
        int action;
        if (hit > stand)
        {
            action = 0;
        }
        else
        {
            action = 1;
        }

        if (action == 0)
        {
            printf("TRIED TO SEND HIT\n");
            if (!sendToServer(client, "HIT"))
            {
                printf("Error sending HIT to server\n");
                return 1;
            }
        }
        else
        {
            if (!sendToServer(client, "STAND"))
            {
                printf("Error sending STAND to server\n");
                return 1;
            }
            break;
        }
    }

    serverMessage = readServerMessage(client);
    if (serverMessage != NULL)
    {
        char *buffer = malloc(strlen(serverMessage) + 1);
        if (sscanf(serverMessage, "RESULT %s", buffer) != 1)
        {
            free(serverMessage);
            printf("Error: Server message is not RESULT");
            return 1;
        }
        free(buffer);
        free(serverMessage);
    }

    return 0;
}

int entry_player(int argc, char *argv[])
{
    // check if there are enough arguments
    if (argc != 5)
    {
        printf("Usage: ./player server_address\n");
        exit(1);
    }

    client_t *client = startClient(argv[1], atoi(argv[2]));

    float ***Q = malloc(sizeof(float **) * 22);

    FILE *fp;
    fp = fopen("qtable.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }
    else
    {
        load_Q(Q, 22, 22, 3);
    }

    // create a string "JOIN" + argv[3]
    // char *joinMessage = malloc(sizeof(char) * 5 + strlen(argv[3]) + 1);
    // strcpy(joinMessage, "JOIN TEAM8");
    // strcat(joinMessage, argv[3]);

    // play game
    if (!sendToServer(client, "JOIN TEAM8\0"))
    {
        printf("Error sending message to server!\n");
        return 1;
    }
    // free(joinMessage);

    int status = 0; // = play_game(Q, client);
    char *serverMessage = readServerMessage(client);

    while (strcmp(serverMessage, "QUIT") != 0)
    {
        status = play_game(Q, client);
        if (status == 1)
        {
            printf("Error playing game!\n");
            free_Q_only(Q, 22, 22, 3);
            return 1;
        }
        serverMessage = readServerMessage(client);
    }

    // close client
    closeClient(client);
    free_Q_only(Q, 22, 22, 3);
    return 0;
}