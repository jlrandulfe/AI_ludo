#ifndef LUDO_PLAYER_CUSTOM_H
#define LUDO_PLAYER_CUSTOM_H
#include <QObject>
#include <iostream>
#include <vector>
#include "positions_and_dice.h"

class ludo_player_custom : public QObject {
    Q_OBJECT
private:
    int n_states = 5;
    int n_actions = 8;
    std::vector< std::vector<int> > R;
    std::vector< std::vector<int> > Q;
    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int dice_roll;
    int make_decision();
public:
    ludo_player_custom();
signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
};

#endif // LUDO_PLAYER_CUSTOM_H
