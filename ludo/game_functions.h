
namespace game_functions {

    bool isGlobe(int index){
        if(index % 13 == 0 || (index - 8) % 13 == 0) {  
            return true;
        }
        return false;
    }

    int isStar(int index){
        if(index == 5  ||
        index == 18 ||
        index == 31 ||
        index == 44){
            return 6;
        } else if(index == 11 ||
                index == 24 ||
                index == 37 ||
                index == 50){
            return 7;
        }
        return 0;
    }

    // bool isStar(int index){
    //     if(index == 5  ||
    //     index == 18 ||
    //     index == 31 ||
    //     index == 44 ||
    //     index == 11 ||
    //     index == 24 ||
    //     index == 37 ||
    //     index == 50) {
    //         return true;
    //     }
    //     return false; 
    // }

    std::pair<int, int> get_next_state_and_action(int init_position,
                                                  int dice_roll,
                                                  std::vector<int> positions) {
        int next_state;
        int action;
        int next_position = init_position + dice_roll;

        // Check for friends at the destination square.
        bool ally = false;
        for (int i=0; i<4; ++i) {
            if (positions[i] == next_position) {
                ally = true;
                break;
            }
        }
        // Check for enemies or a blockade at the destination square.
        bool enemy = false;
        bool blocking = false;
        for (int i=4; i<16; ++i) {
            if (positions[i] == next_position) {
                enemy = true;
                // Check if there is a blocking
                for (int j=4; j<16; ++j) {
                    if (positions[j]==next_position && i!=j) {
                        blocking = true;
                        break;
                    }
                }
                break;
            }
        }

        // Generic state update
        next_state = next_position + 1;

        // Token at start home
        if (init_position == -1) {
            if (dice_roll == 6) {
                action = 0;
                next_state = 1;
            }
            else {
                action = -1;
                next_state = 0;
            }
        }

        // Token already at goal.
        else if (init_position == 99) {
            action = 7;
            next_state = 57;
        }

        // Token to star
        else if (isStar(next_position) != 0) {
            // Last start i.e. directly to goal
            if (next_position == 50) {
                action = 7;
                next_state = 57;
            }
            else{
                action = 3;
                int jump = isStar(next_position);
                next_state = next_position + jump + 1;
            }
            // TODO: Consider situation when:
            // enemy or blocking at the jumped position
        }

        // Token to home stretch
        else if (next_position > 50 && next_position !=56) {
            action = 6;
            if (next_position > 56) {
                next_state = 56 - (next_position-56) + 1;
            }
        }

        // Token to globe
        else if (isGlobe(next_position)) {
            if (enemy == true) {
                action = 4;
                next_state = 0;
            }
            else {
                action = 2;
            }
        }

        // Token to goal
        else if (next_position == 56) {
            action = 7;
        }

        // Token to a friend occupied square
        else if (ally == true) {
            action = 2;
        }

        // Token to an enemy occupied square
        else if (enemy == true) {
            if (blocking == true) {
                action = 4;
                next_state = 0;
            }
            else {
                action = 5;
            }
        }

        // Token to a normal position
        else {
            action = 1;
        }

        return std::make_pair(next_state, action);
    }
}
