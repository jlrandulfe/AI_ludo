#include "ludo_player_custom2.h"
#include "game_functions.h"
#include "q_learning.h"


ludo_player_custom2::ludo_player_custom2() {
    // Set the size and initial values of the R and Q matrices.
    // Resize sets the values automatically to 0.
    this->R.resize(this->n_states, std::vector<int>(this->n_actions));
    this->Q.resize(this->n_states, std::vector<int>(this->n_actions));
    q_learning::create_R_matrix(this->R);
    // q_learning::import_matrix(this->R, "../resources/r-matrix");
    // q_learning::import_matrix(this->Q, "../resources/q-matrix15-final2");
    this->discount_factor = 0.1;
    this->learning_rate = 0.25;
    this->learning = true;
}

int ludo_player_custom2::make_decision(){
    bool debug = false;
    // if (this->learning == false) {
    //     debug = true;
    // }
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
    if (learning == true) {
        for (int i=0; i<4; ++i) {
            if (possible_actions[i] != -1 && this->state[i] != 57) {
                movable_pieces.push_back(i);
            }
        }
    }

    // DEBUG:
    if (debug) {
        std::cout << "\nDice roll: " << dice_roll << "\n";
        std::cout << "Tokens positions:\n";
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

    int select;
    // Select a random piece for moving.
    if (learning == true) {
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
            q_learning::update_Q_matrix(this->Q, this->R, this->discount_factor,
                                        this->state[idx], possible_states[idx],
                                        possible_actions[idx],
                                        this->learning_rate);
        }
        else {
            for(int i = 0; i < 4; ++i){
                if(this->pos_start_of_turn[i] != 99){
                    select = i;
                }
            }
        }
    }

    // If not learning, choose the biggest Q-value
    if (learning == false) {
        std::vector<int> q_values {0, 0, 0, 0};
        for (auto i=0; i<4; ++i) {
            if (this->state[i] != 57) {
                q_values[i] = this->Q[this->state[i]][possible_actions[i]];
            }
            else {
                q_values[i] = -10000;
            }
        }
        if (debug) {
            std::cout << "\nQ-values:\n";
            for (auto t=0; t<q_values.size(); ++t) {
                std::cout << q_values[t] << " ";
            }
        }
        // Look for the maximum q-value
        std::vector<int>::iterator max_it = std::max_element(q_values.begin(),
                                                             q_values.end());
        select = std::distance(q_values.begin(), max_it);
        // // If the q-value is not positive, return a -1.
        // if (q_values[select] <= 0) {
        //     select = -1;
        // }
    }

    //DEBUG
    if (debug) {
        std::cout << "\n";
        std::cout << "\nChosen token: " << select << "\n";
        getchar();
    }
    movable_pieces.clear();
    // Avoid returning -1 due to a bug in the code
    // if (select == -1) {
    //     select = 0;
    // }
    return select;
}

void ludo_player_custom2::start_turn(positions_and_dice relative) {
    this->pos_start_of_turn = relative.pos;
    this->dice_roll = relative.dice;
    int decision = make_decision();
    emit select_piece(decision);
}

void ludo_player_custom2::post_game_analysis(std::vector<int> relative_pos) {
    this->pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(this->pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    emit turn_complete(game_complete);
}


void ludo_player_custom2::get_state() {
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
        else if (this->pos_start_of_turn[i] < 56) {
            this->state[i] = this->pos_start_of_turn[i]+1;
        }
        else if (this->pos_start_of_turn[i] == 99 ||
                 this->pos_start_of_turn[i] == 56) {
            this->state[i] = 57;
        }
        else {
            std::cout << "Pos: " << this->pos_start_of_turn[i] << "\n";
            std::cout << "Error in get_state()\n";
            this->state[i] = 0;
        }
    }
    return;
}