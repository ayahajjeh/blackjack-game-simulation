/*
 * qtable.c - Module for Q table.
 * 
 * Barkin Cavdaroglu, Fall 2022
 * 
 */
#include <stdio.h>
#include <stdlib.h>

float ***Q;
int ***Q_count;

//Q is the average reward for taking an action in a state (state is player and dealer points)
//Q_count is number of times an action has been tried in a state 
//max_player_points: maximum player points
//max_dealer_points: maximum dealer points
//number_actions: number of actions the player can take (2 here, HIT or STAND)
//initialize to zero
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

//player_points: sum of points on player's cards
//dealer_points: point value of dealer's face up card
//action: 0 = HIT, 1 = STAND
//reward: +1 for WIN, -1 for LOOSE or BUST, 0 for PUSH
void update_Q(float ***Q, int ***Q_count, int player_points, int dealer_points, int action, int reward) { 
   Q_count[player_points][dealer_points][action] += 1; //increment count
   Q[player_points][dealer_points][action] += (1/(float)Q_count[player_points][dealer_points][action]) 
                                           * (reward - Q[player_points][dealer_points][action]); //update reward
}

// save qtable to a file
void save_Q(float ***Q, int max_player_points, int max_dealer_points, int number_actions) {
    FILE *fp;
    fp = fopen("qtable.txt", "w");
    int i, j, k;
    for (i = 0; i < max_player_points; i++) {
        for (j = 0; j < max_dealer_points; j++) {
            for (k = 0; k < number_actions; k++) {
                fprintf(fp, "%f ", Q[i][j][k]);
            }
            fprintf(fp, "\n");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

// load qtable from a file
void load_Q(float ***Q, int max_player_points, int max_dealer_points, int number_actions) {
    FILE *fp;
    fp = fopen("qtable.txt", "r");
    int i, j, k;
    for (i = 0; i < max_player_points; i++) {
        Q[i] = malloc(sizeof(float *) * max_dealer_points);
        for (j = 0; j < max_dealer_points; j++) {
            Q[i][j] = malloc(sizeof(float) * number_actions);
            for (k = 0; k < number_actions; k++) {
                fscanf(fp, "%f", &Q[i][j][k]);
            }
        }
    }
    fclose(fp);
}

// free memory allocated to qtable
void free_Q(float ***Q, int ***Q_count, int max_player_points, int max_dealer_points, int number_actions) {
    int i, j;
    for (i = 0; i < max_player_points; i++) {
        for (j = 0; j < max_dealer_points; j++) {
            free(Q[i][j]);
            free(Q_count[i][j]);
        }
        free(Q[i]);
        free(Q_count[i]);
    }
    free(Q);
    free(Q_count);
}

// free memory allocated to qtable
void free_Q_only(float ***Q, int max_player_points, int max_dealer_points, int number_actions) {
    int i, j;
    for (i = 0; i < max_player_points; i++) {
        for (j = 0; j < max_dealer_points; j++) {
            free(Q[i][j]);
        }
        free(Q[i]);
    }
    free(Q);
}