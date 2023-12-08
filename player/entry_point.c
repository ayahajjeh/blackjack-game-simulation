/*
 * entry_point.c - This is the entry point for the player.
 * 
 * usage: ./entry <server_address> <port> <player_name or port> <training=0 or player=1>
 *
 * Barkin Cavdaroglu, Fall 2022
 * 
 */

#include "training.h"
#include "player.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: ./player server_address PORT <PLAYER_NAME or NUMBER_OF_ROUNDS> MODE\n");
        exit(1);
    }

    // playing mode - 0 for training and 1 for playing
    int playing_mode = atoi(argv[4]);

    // player name or number of rounds
    if (playing_mode == 0) {
        entry_training(argc, argv);
    } else {
        entry_player(argc, argv);
    }
}