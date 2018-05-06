#include <iostream>
#include <fstream>

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
}