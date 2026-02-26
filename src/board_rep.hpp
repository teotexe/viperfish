#pragma once

#include "utils.hpp"
#include <cstring>

typedef struct {
    Type::bboard bitboards[12];
    Type::bboard occupancies[3];
    uint8_t castling;
    Type::side stm;
    Type::square enpassant;
    Type::counter hmclock;
    Type::counter fmclock;
} BoardState;

extern BoardState pos;

// extern bboard bitboards[12];
// extern bboard occupancies[3];

extern Type::bboard checkers;
extern Type::bboard pins;

// extern flag castling;

// // Side to move
// extern state stm;

// // En passant square
// extern square enpassant;

// // Half move clock to enforce 50-move rule
// extern counter hmclock;
// // Full move clock to count the total number of moves
// extern counter fmclock;

__always_inline void copy_board_state (BoardState &board) {
    memcpy(board.bitboards, pos.bitboards, 96);
    memcpy(board.occupancies, pos.occupancies, 24);

    board.castling = pos.castling;
    board.enpassant = pos.enpassant;
    board.stm = pos.stm;

    board.hmclock = pos.hmclock;
    board.fmclock = pos.fmclock;
}

__always_inline void restore_board_state (const BoardState &board) {
    memcpy(pos.bitboards, board.bitboards, 96);
    memcpy(pos.occupancies, board.occupancies, 24);

    pos.castling = board.castling;
    pos.enpassant = board.enpassant;
    pos.stm = board.stm;

    pos.hmclock = board.hmclock;
    pos.fmclock = board.fmclock;
}
