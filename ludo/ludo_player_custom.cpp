#include "ludo_player_custom.h"
#include "q_learning.h"
#include "game_functions.h"

ludo_player_custom::ludo_player_custom(Game* my_game) {
    // Set the size and initial values of the R and Q matrices.
    // Resize sets the values automatically to 0.
    this->game = my_game;

    this->R.resize(this->n_states, std::vector<int>(this->n_actions));
    this->Q.resize(this->n_states, std::vector<int>(this->n_actions));
    q_learning::construct_R_matrix(this->R);
    q_learning::get_Q_matrix(this->Q, "../resources/q-matrix");
    // q_learning::set_Q_matrix(this->Q, "../resources/q-matrix-temp");
}

int ludo_player_custom::make_decision(){
    // If all tokens are blocked, don't run the algorithm
    if (std::all_of(std::begin(pos_start_of_turn), std::end(pos_start_of_turn),
            [](int pos){ return pos < 0; })) {
        return 0;
    }
    int possible_actions [4];
    int possible_states [4];
    for (int i=0; i<4; ++i) {
        printf("Got here\n");
        std::pair<int, int> next_move =
                game_functions::get_next_state_and_action(pos_start_of_turn[i],
                        dice_roll, this->game->player_positions);
        possible_states[i] = next_move.first;
        possible_actions[i] = next_move.second;
    }
    return 0;
}

void ludo_player_custom::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
    emit select_piece(decision);
}

void ludo_player_custom::post_game_analysis(std::vector<int> relative_pos){
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    emit turn_complete(game_complete);
}


void ludo_player_custom::get_state() {
    // Update the state of the player tokens:
    // 0: At start position
    // 1: At normal position
    // 2: Blocking
    // 3: At home position
    // 4: At goal
    for (int i=0; i<4; ++i) {
        if (pos_start_of_turn[i]<0) {
            this->state[i] = 0;
        }
        else if (game_functions::isGlobe(pos_start_of_turn[i]) == true) {
            this->state[i] = 2;
        }
        else if (pos_start_of_turn[i] > 51) {
            this->state[i] = 3;
        }
        else if (pos_start_of_turn[i]==99 || pos_start_of_turn[i] == 56) {
            this->state[i] = 4;
        }
        else {
            this->state[i] = 1;
            for (int j=0; j<4; ++j) {
                if (j!=i) {
                    if (pos_start_of_turn[i] == pos_start_of_turn[j]) {
                        this->state[i] = 2;
                    }
                }
            }
        }
    }
    return;
}
