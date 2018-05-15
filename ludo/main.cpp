#include "dialog.h"
#include <iostream>
#include <fstream>
#include <QApplication>
#include "game.h"
#include <vector>
#include "ludo_player.h"
#include "ludo_player_random.h"
#include "ludo_player_custom.h"
#include "positions_and_dice.h"

Q_DECLARE_METATYPE( positions_and_dice )

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

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    qRegisterMetaType<positions_and_dice>();

    Game g;

    //instanciate the players here
    ludo_player_custom p1;
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

    // Successes counter.
    int n_tests = 20;
    int won_games[n_tests] = {};
    double hit_rate[n_tests];
    double total_games = 5000;
    for (int k=0; k<n_tests; ++k) {
        p1.learning_rate = (n_tests-k) / n_tests;
        // Learning loop.
        p1.learning = true;
        for(int i = 0; i < total_games; ++i){
            g.start();
            a.exec();
            std::cout << i << std::endl;
            g.reset();
        }
        // Testing loop.
        p1.learning = false;
        for(int i = 0; i < total_games; ++i){
            g.start();
            a.exec();
            if (g.color == 0) {
                won_games[k] += 1;
            }
            std::cout << i << std::endl;
            g.reset();
        }
        hit_rate[k] = 100 * won_games[k] / total_games;
    }

    // Write the resulting hit rates to a file.
    std::ofstream results_file("../resources/results");
    for(int test=0; test<n_tests; ++test){
        results_file << hit_rate[test] << " ";
    }
    set_Q_matrix(p1.Q, "../resources/q-matrix-final");
    return 0;
}
