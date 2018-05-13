#include "ludo_player_custom.h"
#include "game_functions.h"
#include "q_learning.h"


ludo_player_custom::ludo_player_custom() {
    // Set the size and initial values of the R and Q matrices.
    // Resize sets the values automatically to 0.
    this->R.resize(this->n_states, std::vector<int>(this->n_actions));
    this->Q.resize(this->n_states, std::vector<int>(this->n_actions));
    q_learning::construct_R_matrix(this->R);
    q_learning::get_Q_matrix(this->Q, "../resources/q-matrix");
    this->discount_factor = 0.5;
    this->learning = true;
}

int ludo_player_custom::make_decision(){
    bool debug = false;
    // If all tokens are blocked, don't run the algorithm
    if (std::all_of(std::begin(this->pos_start_of_turn),
                    std::end(this->pos_start_of_turn),
                    [](int pos){ return pos < 0; })) {
        return 0;
    }
    // Update the state of the player.
    this->get_state();
    int possible_actions [4];
    int possible_states [4];
    // Get the next state, action, and reward for moving each token.
    for (int i=0; i<4; ++i) {
        std::pair<int, int> next_move =
                game_functions::get_next_state_and_action(
                        this->pos_start_of_turn[i], this->dice_roll,
                        this->pos_start_of_turn);
        possible_states[i] = next_move.first;
        possible_actions[i] = next_move.second;
    }
    // Make a list with the pieces that can be moved.
    std::vector<int> movable_pieces;
    for (int i=0; i<4; ++i) {
        if (possible_actions[i] != -1) {
            movable_pieces.push_back(i);
        }
    }

    // DEBUG:
    if (debug) {
        std::cout << "Dice roll: " << dice_roll << "\n";
        std::cout << "Tokens positions:\n";
        std::cout << "Positions:\n";
        for (int i=0; i<4; ++i) {
            std::cout << this->pos_start_of_turn[i] << " ";
        }
        std::cout << "\nStates:\n";
        for (int i=0; i<4; ++i) {
            std::cout << this->state[i] << " ";
        }
        std::cout << "\nPossible actions:\n";
        for (int i=0; i<4; ++i) {
            std::cout << possible_actions[i] << " ";
        }
        std::cout << "\nPossible states:\n";
        for (int i=0; i<4; ++i) {
            std::cout << possible_states[i] << " ";
        }
        std::cout << "\n";
    }

    // Select a random piece for moving.
    int select;
    if (movable_pieces.size() > 0) {
        if (debug) {
            std::cout << "\nMovable tokens:\n";
                for (auto i=0; i<movable_pieces.size(); ++i) {
                    std::cout << movable_pieces[i] << " ";
                }
        }
        std::uniform_int_distribution<> piece(0, movable_pieces.size()-1);
        int idx = piece(gen);
        select = movable_pieces[idx];
        //DEBUG
        if (debug) {
            std::cout << "\nChosen token: " << select << "\n";
        }
        q_learning::update_Q_matrix(this->Q, this->R, this->discount_factor,
                                    this->state[idx], possible_states[idx],
                                    possible_actions[idx]);
    }
    else {
        select = -1;
    }

    if (debug){
        std::cout << "\n";
        // getchar();
    }

    movable_pieces.clear();
    return select;
}

void ludo_player_custom::start_turn(positions_and_dice relative) {
    this->pos_start_of_turn = relative.pos;
    this->dice_roll = relative.dice;
    int decision = make_decision();
    emit select_piece(decision);
}

void ludo_player_custom::post_game_analysis(std::vector<int> relative_pos) {
    this->pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(this->pos_end_of_turn[i] < 99){
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
        if (this->pos_start_of_turn[i]<0) {
            this->state[i] = 0;
        }
        else if (this->pos_start_of_turn[i] > 51 &&
                 this->pos_start_of_turn[i] < 56) {
            this->state[i] = 3;
        }
        else if (this->pos_start_of_turn[i] == 99 ||
                 this->pos_start_of_turn[i] == 56) {
            this->state[i] = 4;
        }
        else if (game_functions::isGlobe(this->pos_start_of_turn[i]) == true) {
            this->state[i] = 2;
        }
        else {
            this->state[i] = 1;
            for (int j=0; j<4; ++j) {
                if (j!=i) {
                    if (this->pos_start_of_turn[i] ==
                            this->pos_start_of_turn[j]) {
                        this->state[i] = 2;
                    }
                }
            }
        }
    }
    return;
}
