#pragma once

#include <iostream>
#include "move_gen.hpp"
#include "test_utils.hpp"
#include "fen.hpp"

uint64_t run_perft (uint8_t depth);
void run_user_perft (uint8_t depth);
bool run_perfts ();

inline void perft_driver(uint8_t depth, uint64_t &nodes) {
    Moves move_list;
    generate_moves(move_list);

    if (depth == 1) {
        nodes += move_list.index;
        return;
    }

    for (uint8_t i = 0; i < move_list.index; i++) {
        move mv = move_list.moves[i];

        // Copy board state
        BoardState board;
        copy_board_state(board);

        // Make move
        make_move(mv);

        // Continue recursively
        perft_driver(depth - 1, nodes);

        // Restore board state
        restore_board_state(board);
    }
}
