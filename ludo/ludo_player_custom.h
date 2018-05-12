#ifndef LUDO_PLAYER_CUSTOM_H
#define LUDO_PLAYER_CUSTOM_H
#include <QObject>
#include <iostream>
#include <vector>
#include <random>
#include "positions_and_dice.h"
#include "game.h"




class ludo_player_custom : public QObject {
    Q_OBJECT
private:
    Game* game;
    // Q-learning attributes
    int state [4];
    int n_states = 5;
    int n_actions = 8;
    std::vector< std::vector<int> > R;
    std::vector< std::vector<int> > Q;
    double discount_factor;
    bool learning;
    // Ludo game attributes
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int dice_roll;
    std::mt19937 gen;

    int make_decision();
    void get_state();
public:
    ludo_player_custom(Game* game);
signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};

#endif // LUDO_PLAYER_CUSTOM_H
