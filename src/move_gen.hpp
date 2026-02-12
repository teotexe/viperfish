#pragma once

#include "move_gen_helper.hpp"

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

// Generate all legal moves
__always_inline void generate_moves (Moves &move_list) {
    move_list.index = 0;

    find_checkers(!stm);
    if (stm == white) {
        if (countbits(checkers) > 1) {
            generate_moves_king(move_list, white);
            return;
        }

        if (countbits(checkers) == 1) {
            square king_sq = getls1b(bitboards[K]);
            square checker_sq = getls1b(checkers);
            legal_mv_mask = blocker_tables[king_sq][checker_sq] | checkers;
        } else {
            legal_mv_mask = 0xFFFFFFFFFFFFFFFFULL;
        }

        // Compute pins and pins_mask
        find_pins(stm);

        for (piece curr_piece = P; curr_piece <= Q; curr_piece++) {
            if (curr_piece == P) {
                generate_moves_white_pawn(move_list);
            }
            if (curr_piece == R) {
                generate_moves_rook(move_list, white);
            }
            if (curr_piece == N) {
                generate_moves_knight(move_list, white);
            }
            if (curr_piece == B) {
                generate_moves_bishop(move_list, white);
            }
            if (curr_piece == K) {
                generate_moves_king(move_list, white);
            }
            if (curr_piece == Q) {
                generate_moves_queen(move_list, white);
            }
        }
    } else {
        if (countbits(checkers) > 1) {
            generate_moves_king(move_list, black);
            return;
        }

        if (countbits(checkers) == 1) {
            square king_sq = getls1b(bitboards[k]);
            square checker_sq = getls1b(checkers);
            legal_mv_mask = blocker_tables[king_sq][checker_sq] | checkers;
        } else {
            legal_mv_mask = 0xFFFFFFFFFFFFFFFFULL;
        }

        // Compute pins and pins_mask
        find_pins(stm);

        for (piece curr_piece = p; curr_piece <= q; curr_piece++) {
            if (curr_piece == p) {
                generate_moves_black_pawn(move_list);
            }
            if (curr_piece == r) {
                generate_moves_rook(move_list, black);
            }
            if (curr_piece == n) {
                generate_moves_knight(move_list, black);
            }
            if (curr_piece == b) {
                generate_moves_bishop(move_list, black);
            }
            if (curr_piece == k) {
                generate_moves_king(move_list, black);
            }
            if (curr_piece == q) {
                generate_moves_queen(move_list, black);
            }
        }
    }
}

constexpr uint8_t update_castling_rights[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14,
};

// Make one move
__always_inline int make_move(int move) {
    int source = get_mv_src(move);
    int target = get_mv_trgt(move);
    int piece = get_mv_piece(move);
    int promoted = get_mv_prmtd(move);
    int capture = get_mv_cptr(move);
    int double_move = get_mv_dblpsh(move);
    int en_passant_move = get_mv_enpsnt(move);
    int castling = get_mv_cstlng(move);

    // Refs to occupancies bboards
    bboard *own_occ = &occupancies[stm];
    bboard *opp_occ = &occupancies[stm ^ 1];

    // Move the piece
    popbit(bitboards[piece], source);
    setbit(bitboards[piece], target);

    // Update own occupancy
    popbit(*own_occ, source);
    setbit(*own_occ, target);

    // Handle captures (including normal capture on target)
    if (capture) {
        int cap_sq = target;
        // En passant captures use different square
        if (en_passant_move) cap_sq = (stm == white) ? (target + 8) : (target - 8);

        // Remove from opponent occupancies
        popbit(*opp_occ, cap_sq);

        // Find the piece on the capture square 
        // and clear its bitboard
        int start = (stm == white) ? p : P;
        int end   = (stm == white) ? q : Q;
        for (int bb = start; bb <= end; bb++) {
            if (getbit(bitboards[bb], cap_sq)) {
                popbit(bitboards[bb], cap_sq);
                break;
            }
        }
    }

    // Handle promotion
    if (promoted) {
        int pawn = (stm == white) ? P : p;
        
        // Remove the pawn
        popbit(bitboards[pawn], target);
        popbit(*own_occ, target);
        
        // Promote it
        setbit(bitboards[promoted], target);
        setbit(*own_occ, target);
    }

    // ???????????
    // // Handle en passant (when move had en passant flag but not capture flag)
    // if (en_passant_move && !capture) {
    //     // shouldn't normally happen, but handle defensively
    //     int cap_sq = (stm == white) ? (target + 8) : (target - 8);
    //     popbit(*opp_occ, cap_sq);
    //     int start = (stm == white) ? p : P;
    //     int end   = (stm == white) ? q : Q;
    //     for (int bb = start; bb <= end; bb++) {
    //         if (getbit(bitboards[bb], cap_sq)) {
    //             popbit(bitboards[bb], cap_sq);
    //             break;
    //         }
    //     }
    // }
    // ???????????

    // Reset en passant and set if double pawn push
    enpassant = no_sq;
    if (double_move) enpassant = (stm == white) ? target + 8 : target - 8;

    // Handle castling (update rook bitboard and own occupancy)
    if (castling) {
        switch (target) {
            case g1:
                popbit(bitboards[R], h1);
                setbit(bitboards[R], f1);
                popbit(*own_occ, h1);
                setbit(*own_occ, f1);
                break;
            case c1:
                popbit(bitboards[R], a1);
                setbit(bitboards[R], d1);
                popbit(*own_occ, a1);
                setbit(*own_occ, d1);
                break;
            case g8:
                popbit(bitboards[r], h8);
                setbit(bitboards[r], f8);
                popbit(*own_occ, h8);
                setbit(*own_occ, f8);
                break;
            case c8:
                popbit(bitboards[r], a8);
                setbit(bitboards[r], d8);
                popbit(*own_occ, a8);
                setbit(*own_occ, d8);
                break;
            default:
                break;
        }
    }

    // Update castling rights
    castling &= update_castling_rights[source];
    castling &= update_castling_rights[target];

    // Update combined occupancy
    occupancies[both] = occupancies[white] | occupancies[black];

    // Switch side
    stm ^= 1;

    return 1;
}