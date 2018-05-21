#include <iostream>
#include <fstream>
#ifndef QLEARNING_H
#define QLEARNING_H

namespace q_learning {
    void import_matrix(std::vector< std::vector<int> >& matrix,
                       std::string fname) {
        std::ifstream matrix_file(fname);
        for(int row=0; row<58; ++row){
            for(int col=0; col<8; ++col){
                matrix_file >> matrix[row][col];
            }
        }
        matrix_file.close();
        return;
    }

    void export_matrix(std::vector< std::vector<int> >& matrix,
                       std::string fname) {
        std::ofstream matrix_file(fname);
        for(int row=0; row<5; ++row){
            for(int col=0; col<8; ++col){
                matrix_file << matrix[row][col] << " ";
            }
            matrix_file << "\n";
        }
        matrix_file.close();
        return;
    }

    void create_R_matrix(std::vector< std::vector<int> >& R) {
        R[0] = {90, 0, 0, 0, 0, 0, 0, 0};
        for (int i=1; i<44+1; ++i) {
            R[i] = {0, 10, 20, 30, -100, 50, 0, 0};
        }
        R[44+1] = {0, 10, 20, 0, -100, 50, 0, 100};
        for (int i=45+1; i<51+1; ++i) {
            R[i] = {0, 10, 20, 0, -100, 50, 200, 300};
        }
        for (int i=51+1; i<56+1; ++i) {
            R[i] = {0, 0, 0, 0, 0, 0, 2, 5};
        }
        R[56+1] = {0, 0, 0, 0, 0, 0, 0, 30};
        return;
    }

    void update_Q_matrix(std::vector< std::vector<int> >& Q,
                         std::vector< std::vector<int> >& R,
                         double discount_factor, int state,
                         int next_state, int action, double alpha) {
        bool debug = false;
        // Get the reward for the selected state and action.
        double reward = R[state][action];
        // Look for the maximum q-value in the future state.
        std::vector<int> Q_future = Q[next_state];
        auto it = max_element(std::begin(Q_future), std::end(Q_future));
        double max_q = *it;
        // Bellman equation. 
        double target = reward + discount_factor*max_q;
        double new_q = (1-alpha)*Q[state][action] + alpha*target;
        Q[state][action] = (int)new_q;

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
