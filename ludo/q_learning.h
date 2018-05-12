#include <iostream>
#include <fstream>
#ifndef QLEARNING_H
#define QLEARNING_H

namespace q_learning {
    void construct_R_matrix(std::vector< std::vector<int> >& R) {
        R[0] = {20, 0, 0, 0, 0, 0, 0, 0};
        R[1] = {0, 10, 20, 30, -10, 50, 40, 60};
        R[2] = {0, 2, 5, 20, -10, 40, 30, 50};
        R[3] = {0, 0, 0, 0, 0, 0, 2, 5};
        R[4] = {0, 0, 0, 0, 0, 0, 0, 1000};
        return;
    }

    void get_Q_matrix(std::vector< std::vector<int> >& Q, std::string fname) {
        std::ifstream matrix_file(fname);
        for(int row=0; row<5; ++row){
            for(int col=0; col<8; ++col){
                matrix_file >> Q[row][col];
            }
        }
        matrix_file.close();
        return;
    }

    void set_Q_matrix(std::vector< std::vector<int> >& Q, std::string fname) {
        std::ofstream matrix_file(fname);
        for(int row=0; row<5; ++row){
            for(int col=0; col<8; ++col){
                matrix_file << Q[row][col] << " ";
            }
            matrix_file << "\n";
        }
        matrix_file.close();
        return;
    }

    void update_Q_matrix(std::vector< std::vector<int> >& Q,
                         std::vector< std::vector<int> >& R,
                         double discount_factor, int state,
                         int next_state, int action) {
        bool debug = false;
        // Get the reward for the selected state and action.
        int reward = R[state][action];
        // Look for the maximum q-value in the future state.
        std::vector<int> Q_future = Q[next_state];
        auto it = max_element(std::begin(Q_future), std::end(Q_future));
        int max_q = *it;
        // Bellman equation.
        int new_q = reward + discount_factor*max_q;
        Q[state][action] = new_q;

        //DEBUG
        if (debug) {
            std::cout << "Reward: " << reward << "\n";
            std::cout << "Future state q-values:\n";
            for (int i=0; i<Q_future.size(); ++i) {
                std::cout << Q_future[i] << " ";
            }
            std::cout << "\nmax-q: " << max_q << "\n";
            std::cout << "new-q: " << new_q << "\n";
        }
    }
}

#endif
