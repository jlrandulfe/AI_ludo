
namespace q_learnning {
    void construct_R_matrix(std::vector< std::vector<int> >& R) {
        R[0] = {20, 0, 0, 0, 0, 0, 0, 0};
        R[1] = {0, 10, 20, 30, -10, 50, 40, 60};
        R[2] = {0, 2, 5, 20, -10, 40, 30, 50};
        R[3] = {0, 0, 0, 0, 0, 0, 2, 5};
        R[4] = {0, 0, 0, 0, 0, 0, 0, 1000};
        return;
    }
}