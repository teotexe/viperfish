#pragma once

#include "utils.hpp"

// Binary move's representation:

//    0000 0000 0000 0000 00XX XXXX   Source square       (Up to 6b)
//    0000 0000 0000 XXXX XX00 0000   Target square       (Up to 6b)
//    0000 0000 XXXX 0000 0000 0000   Piece               (Up to 4b)
//    0000 XXXX 0000 0000 0000 0000   Promoted piece      (Up to 4b)
//    000X 0000 0000 0000 0000 0000   Capture flag        (Up to 1b)
//    00X0 0000 0000 0000 0000 0000   Double push flag    (Up to 1b)
//    0X00 0000 0000 0000 0000 0000   En passant flag     (Up to 1b)
//    X000 0000 0000 0000 0000 0000   Castling flag       (Up to 1b)

constexpr uint32_t source_sq_mask = 0x00003F;
constexpr uint32_t target_sq_mask = 0x000FC0;
constexpr uint32_t piece_mask = 0x00F000;
constexpr uint32_t promoted_piece_mask = 0x0F0000;
constexpr uint32_t capture_flag_mask = 0x100000;
constexpr uint32_t double_push_flag_mask = 0x200000;
constexpr uint32_t en_passant_flag_mask = 0x300000;
constexpr uint32_t castling_flag_mask = 0x400000;

__always_inline square get_mv_src (move mv) {
    return mv & source_sq_mask;
}

__always_inline square get_mv_trgt (move mv) {
    return (mv & target_sq_mask) >> 6;
}

__always_inline piece get_mv_piece (move mv) {
    return (mv & piece_mask) >> 12;
}

__always_inline piece get_mv_prmtd (move mv) {
    return (mv & promoted_piece_mask) >> 16;
}

__always_inline flag get_mv_cptr (move mv) {
    return (mv & capture_flag_mask) >> 20;
}

__always_inline flag get_mv_dblpsh (move mv) {
    return (mv & double_push_flag_mask) >> 21;
}

__always_inline flag get_mv_enpsnt (move mv) {
    return (mv & en_passant_flag_mask) >> 22;
}

__always_inline flag get_mv_cstlng (move mv) {
    return (mv & castling_flag_mask) >> 23;
}
