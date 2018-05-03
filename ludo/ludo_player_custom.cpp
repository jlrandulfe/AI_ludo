#include "ludo_player_custom.h"
#include "q_learning.h"
#include <random>

ludo_player_custom::ludo_player_custom(){
    // Set the size and initial values of the R and Q matrices.
    // Resize sets the values automatically to 0.
    this->R.resize(this->n_states, std::vector<int>(this->n_actions));
    this->Q.resize(this->n_states, std::vector<int>(this->n_actions));
    q_learnning::construct_R_matrix(this->R);
}

int ludo_player_custom::make_decision(){
    if(dice_roll == 6){
        // Move the token from home in case any of them is there
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
        // Move the token with the higher index
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
    } else {
        // Move the token with the higher index
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
        // All tokens at home
        for(int i = 0; i < 4; ++i){ //maybe they are all locked in
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
    }
    return -1;
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
