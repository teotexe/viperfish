#include "engine.hpp"

void init_board_state () {
    Attacks::init();
    create_blocker_tables(blocker_tables);
}
