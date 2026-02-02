#pragma once

#include "bit_manip.hpp"
#include "utils.hpp"

extern bboard pawn_att[2][64];
extern bboard knight_att[64];
extern bboard king_att[64];

extern bboard bishop_att[64][512];
extern bboard rook_att[64][4096];

void create_pawn_attacks (bboard (&bb)[2][64]);
void create_knight_attacks (bboard (&bb)[64]);
void create_king_attacks (bboard (&bb)[64]);
void create_bishop_masks (bboard (&bb)[64]);
void create_rook_masks (bboard (&bb)[64]);
void create_bishop_attacks (bboard (&bb)[64][512]);
void create_rook_attacks (bboard (&bb)[64][4096]);
