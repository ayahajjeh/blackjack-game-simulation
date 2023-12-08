typedef struct state {
    int handValue;
    int dealerCardValue;
    int action;
} state_t;

//Q is the average reward for taking an action in a state (state is player and dealer points)
//Q_count is number of times an action has been tried in a state 
//max_player_points: maximum player points
//max_dealer_points: maximum dealer points
//number_actions: number of actions the player can take (2 here, HIT or STAND)
//initialize to zero
void initialize_Q(float ***Q, int ***Q_count, int max_player_points, int max_dealer_points, int number_actions);

//player_points: sum of points on player's cards
//dealer_points: point value of dealer's face up card
//action: 0 = HIT, 1 = STAND
//reward: +1 for WIN, -1 for LOOSE or BUST, 0 for PUSH
void update_Q(float ***Q, int ***Q_count, int player_points, int dealer_points, int action, int reward);

void save_Q(float ***Q, int max_player_points, int max_dealer_points, int number_actions);

void load_Q(float ***Q, int max_player_points, int max_dealer_points, int number_actions);

void free_Q(float ***Q, int ***Q_count, int max_player_points, int max_dealer_points, int number_actions);

void free_Q_only(float ***Q, int max_player_points, int max_dealer_points, int number_actions);
