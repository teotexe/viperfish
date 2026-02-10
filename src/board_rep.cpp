#include "board_rep.hpp"

bboard bitboards[12];
bboard occupancies[3];

bboard checkers;
bboard pins;

flag castling;

// Side to move
state stm;

// En passant square
square enpassant;

// Half move clock to enforce 50-move rule
counter hmclock;
