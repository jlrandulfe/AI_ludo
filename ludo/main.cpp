#include "dialog.h"
#include <iostream>
#include <fstream>
#include <QApplication>
#include "game.h"
#include <vector>
#include "ludo_player.h"
#include "ludo_player_random.h"
#include "ludo_player_custom2.h"
#include "positions_and_dice.h"

Q_DECLARE_METATYPE( positions_and_dice )

void set_Q_matrix(std::vector< std::vector<int> >& Q, std::string fname) {
    std::ofstream matrix_file(fname);
    for(int row=0; row<58; ++row){
        for(int col=0; col<8; ++col){
            matrix_file << Q[row][col] << " ";
        }
        matrix_file << "\n";
    }
    matrix_file.close();
    return;
}

int matrix_total(std::vector< std::vector<int> > matrix) {
    int total_sum = 0;
    int n_rows = matrix.size();
    for (int row=0; row<n_rows; ++row) {
        for (auto& n : matrix[row])
            total_sum += n;
    }
    return total_sum;
}

int matrix_difference(std::vector< std::vector<int> > matrix1,
                      std::vector< std::vector<int> > matrix2) {
    int diff = 0;
    int n_rows = matrix1.size();
    int n_cols = matrix1[0].size();
    for (int row=0; row<n_rows; ++row) {
        std::vector<int> row_diff;
        row_diff.resize(n_cols);
        std::transform (matrix1[row].begin(), matrix1[row].end(),
                        matrix2[row].begin(), row_diff.begin(),
                        std::minus<int>());
        for (auto& n : row_diff)
            if (n<0) {
                diff -= n;
            }
            else {
                diff += n;   
            }
    }
    return diff;
}

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    qRegisterMetaType<positions_and_dice>();

    Game g;

    //instanciate the players here
    ludo_player_custom2 p1;
    // p1.player_positions = &g.player_positions;
    ludo_player_random p2, p3, p4;

    g.setGameDelay(0); //if you want to see the game, set a delay

    /*/ Add a GUI <-- remove the '/' to uncomment block
    Dialog w;
    QObject::connect(&g,SIGNAL(update_graphics(std::vector<int>)),&w,SLOT(update_graphics(std::vector<int>)));
    QObject::connect(&g,SIGNAL(set_color(int)),                   &w,SLOT(get_color(int)));
    QObject::connect(&g,SIGNAL(set_dice_result(int)),             &w,SLOT(get_dice_result(int)));
    QObject::connect(&g,SIGNAL(declare_winner(int)),              &w,SLOT(get_winner()));
    QObject::connect(&g,SIGNAL(close()),&a,SLOT(quit()));
    w.show();
    /*/ //Or don't add the GUI
    QObject::connect(&g,SIGNAL(close()),&a,SLOT(quit()));
    //*/

    //set up for each player
    QObject::connect(&g, SIGNAL(player1_start(positions_and_dice)),&p1,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p1,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player1_end(std::vector<int>)),    &p1,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p1,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    QObject::connect(&g, SIGNAL(player2_start(positions_and_dice)),&p2,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p2,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player2_end(std::vector<int>)),    &p2,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p2,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    QObject::connect(&g, SIGNAL(player3_start(positions_and_dice)),&p3,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p3,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player3_end(std::vector<int>)),    &p3,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p3,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    QObject::connect(&g, SIGNAL(player4_start(positions_and_dice)),&p4,SLOT(start_turn(positions_and_dice)));
    QObject::connect(&p4,SIGNAL(select_piece(int)),                &g, SLOT(movePiece(int)));
    QObject::connect(&g, SIGNAL(player4_end(std::vector<int>)),    &p4,SLOT(post_game_analysis(std::vector<int>)));
    QObject::connect(&p4,SIGNAL(turn_complete(bool)),              &g, SLOT(turnComplete(bool)));

    std::vector< std::vector<int> > prev_Q;
    prev_Q.resize(p1.Q.size(), std::vector<int>(p1.Q[0].size()));
    // Successes counter.
    int n_tests = 10;
    int won_games0 = 0;
    int won_games1 = 0;
    int won_games2 = 0;
    int won_games3 = 0;
    double hit_rate0;
    double hit_rate1;
    double hit_rate2;
    double hit_rate3;
    double test_games = 1000;
    double learning_games = 100000;
    int diffs[(int)learning_games] = {};
    // Learning loop.
    p1.learning = true;
    for(int i = 0; i < learning_games; ++i){
        g.start();
        a.exec();
        g.reset();
        diffs[i] = matrix_total(p1.Q);
        prev_Q = p1.Q;
    }
    // Testing loop.
    p1.learning = false;
    for(int i = 0; i < test_games; ++i){
        g.start();
        a.exec();
        if (g.color == 0) {
            won_games0 += 1;
        }
        else if (g.color == 1) {
            won_games1 += 1;
        }
        else if (g.color == 2) {
            won_games2 += 1;
        }
        else if (g.color == 3) {
            won_games3 += 1;
        }
        else {
            std::cout << "\nUnknown player won\n\n";
        }
        g.reset();
    }
    hit_rate0 = 100 * won_games0 / test_games;
    hit_rate1 = 100 * won_games1 / test_games;
    hit_rate2 = 100 * won_games2 / test_games;
    hit_rate3 = 100 * won_games3 / test_games;

    // Write the resulting hit rates to a file.
    std::cout << "Won rate p1: " << hit_rate0 << "\n";
    std::cout << "Won rate p2: " << hit_rate1 << "\n";
    std::cout << "Won rate p3: " << hit_rate2 << "\n";
    std::cout << "Won rate p4: " << hit_rate3 << "\n";

    set_Q_matrix(p1.Q, "../resources/q-matrix-v2-final");

    // Write the tendency of the Q matrix change to a file
    std::ofstream q_evo_file("../resources/results_q-evolution");
    for(int run=0; run<learning_games; ++run){
        q_evo_file << diffs[run] << " ";
    }



    // // Successes counter.
    // int n_tests = 20;
    // int won_games[n_tests] = {};
    // double hit_rate[n_tests];
    // double total_games = 5000;

    // // Discount factor tests
    // for (int k=0; k<n_tests; ++k) {
    //     std::cout << "Test " << k << "\n";
    //     p1.discount_factor = (n_tests-k) / (0.5*(double)n_tests);
    //     // Learning loop.
    //     p1.learning = true;
    //     for(int i = 0; i < total_games; ++i){
    //         g.start();
    //         a.exec();
    //         // std::cout << i << std::endl;
    //         g.reset();
    //     }
    //     // Testing loop.
    //     p1.learning = false;
    //     for(int i = 0; i < total_games; ++i){
    //         g.start();
    //         a.exec();
    //         if (g.color == 0) {
    //             won_games[k] += 1;
    //         }
    //         std::cout << i << std::endl;
    //         g.reset();
    //     }
    //     hit_rate[k] = 100 * won_games[k] / total_games;
    //     set_Q_matrix(p1.Q, "../resources/q-matrix"+std::to_string(k)+"-final");

    //     // Reset the Q matrix to zeros.
    //     for (auto& sub : p1.Q) {
    //         std::fill(sub.begin(), sub.end(), 0);
    //     }
    // }

    // // // Write the resulting hit rates to a file.
    // std::ofstream results_file("../resources/results_discount-factor");
    // for(int test=0; test<n_tests; ++test){
    //     results_file << hit_rate[test] << " ";
    // }



    // int won_games2[n_tests] = {};
    // // Learning rate tests
    // p1.discount_factor = 0.3;
    // for (int k=0; k<n_tests; ++k) {
    //     std::cout << "Test " << k << "\n";
    //     p1.learning_rate = (n_tests-k) / ((double)n_tests);
    //     // Learning loop.
    //     p1.learning = true;
    //     for(int i = 0; i < total_games; ++i){
    //         g.start();
    //         a.exec();
    //         // std::cout << i << std::endl;
    //         g.reset();
    //     }
    //     // Testing loop.
    //     p1.learning = false;
    //     for(int i = 0; i < total_games; ++i){
    //         g.start();
    //         a.exec();
    //         if (g.color == 0) {
    //             won_games2[k] += 1;
    //         }
    //         std::cout << i << std::endl;
    //         g.reset();
    //     }
    //     hit_rate[k] = 100 * won_games2[k] / total_games;
    //     set_Q_matrix(p1.Q, "../resources/q-matrix"+std::to_string(k)+"-final2");

    //     // Reset the Q matrix to zeros.
    //     for (auto& sub : p1.Q) {
    //         std::fill(sub.begin(), sub.end(), 0);
    //     }
    // }

    // // Write the resulting hit rates to a file.
    // std::ofstream results_file2("../resources/results_learning-rate");
    // for(int test=0; test<n_tests; ++test){
    //     results_file2 << hit_rate[test] << " ";
    // }

    // set_Q_matrix(p1.R, "../resources/rewards2");

    return 0;
}
