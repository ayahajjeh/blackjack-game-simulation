#include "cards.h"

card_t read_card(char *serverMessage, int counter);
void play_game_train(float ***Q, int ***Q_count, char *server_address, int PORT);
void training(const char *server_address, int PORT, int number_of_rounds);
int entry_training(int argc, char* argv[]);
