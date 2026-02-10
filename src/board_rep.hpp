#pragma once

#include "utils.hpp"

extern bboard bitboards[12];
extern bboard occupancies[3];

extern bboard checkers;
extern bboard pins;

extern flag castling;

// Side to move
extern state stm;

// En passant square
extern square enpassant;

// Half move clock to enforce 50-move rule
extern counter hmclock;
