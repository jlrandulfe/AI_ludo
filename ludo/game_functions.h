
namespace game_functions {

    bool isGlobe(int index){
        if(index % 13 == 0 || (index - 8) % 13 == 0) {  
            return true;
        }
        return false;
    }

    bool isStar(int index){
        if(index == 5  ||
        index == 18 ||
        index == 31 ||
        index == 44 ||
        index == 11 ||
        index == 24 ||
        index == 37 ||
        index == 50) {
            return true;
        }
        return false; 
    }

    std::pair<int, int> get_next_state_and_action(int init_position,
                                                  int dice_roll,
                                                  std::vector<int> positions) {
        int next_state;
        int action;
        int next_position = init_position + dice_roll;
        // DEBUG
        std::cout << "P2: " << positions[4] << ", " << positions[5] 
                << ", " << positions[6] << ", " << positions[7] << "\n";
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
        // Token to star
        else if (isStar(next_position)) {
            action = 3;
            next_state = 1;
        }
        // Token to globe
        else if (isGlobe(next_position)) {
            action = 2;
            next_state = 2;
        }
        // Token to a friend occupied square

        // Token to an enemy occupied square

        // Token to home row
        else if (next_position > 50) {
            action = 6;
            next_state = 3;
        }
        // Token to goal
        else if (next_position == 56 || next_position == 50) {
            action = 7;
            next_state = 4;
        }
        // Token to a normal position
        else {
            action = 1;
            next_state = 1;
        }
        return std::make_pair(next_state, action);
    }
}