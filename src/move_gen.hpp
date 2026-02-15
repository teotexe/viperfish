#pragma once

#include "move_gen_helper.hpp"

constexpr uint8_t update_castling_rights[64] = {
    13, 15, 15, 15, 12, 15, 15, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
     7, 15, 15, 15,  3, 15, 15, 11,
};

// Generate all legal moves
__always_inline void generate_moves (Moves &move_list) {
    move_list.index = 0;

    find_checkers(!stm);

    pawn_mv_mask = 0ULL;

    if (stm == white) {
        if (countbits(checkers) > 1) {
            generate_moves_king(move_list, white);
            return;
        }

        if (countbits(checkers) == 1) {
            square king_sq = getls1b(bitboards[K]);
            square checker_sq = getls1b(checkers);
            legal_mv_mask = blocker_tables[king_sq][checker_sq] | checkers;
            if (enpassant != no_sq && (1ULL << (enpassant - 8)) & checkers) pawn_mv_mask |= (1ULL << enpassant);
        } else {
            legal_mv_mask = 0xFFFFFFFFFFFFFFFFULL;
        }
        pawn_mv_mask |= legal_mv_mask;

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
            if (enpassant != no_sq && (1ULL << (enpassant + 8)) & checkers) pawn_mv_mask |= (1ULL << enpassant);
        } else {
            legal_mv_mask = 0xFFFFFFFFFFFFFFFFULL;
        }
        pawn_mv_mask |= legal_mv_mask;

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

// Make one move
__always_inline int make_move (int move) {
    square source = get_mv_src(move);
    square target = get_mv_trgt(move);
    piece curr_piece = get_mv_piece(move);
    piece promoted = get_mv_prmtd(move);
    flag capture = get_mv_cptr(move);
    flag double_move = get_mv_dblpsh(move);
    flag en_passant_move = get_mv_enpsnt(move);
    flag castling_flag = get_mv_cstlng(move);

    // Refs to occupancies bboards
    bboard *own_occ = &occupancies[stm];
    bboard *opp_occ = &occupancies[!stm];

    // Move the piece
    popbit(bitboards[curr_piece], source);
    setbit(bitboards[curr_piece], target);

    // Update own occupancy
    popbit(*own_occ, source);
    setbit(*own_occ, target);

    // Handle captures (including normal capture on target)
    if (capture) {
        int cap_sq = target;
        // En passant captures use different square
        if (en_passant_move) cap_sq = (stm == white) ? (target - 8) : (target + 8);

        // Remove from opponent occupancies
        popbit(*opp_occ, cap_sq);

        // Find the opponent's piece on the capture square and remove it

        // Loop over opposite side's pieces
        piece start = (stm == white) ? p : P;
        piece end   = (stm == white) ? q : Q;
        for (piece pc = start; pc <= end; pc++) {
            if (getbit(bitboards[pc], cap_sq)) {
                popbit(bitboards[pc], cap_sq);
                break;
            }
        }
    }

    // Handle promotion
    if (promoted) {
        piece pawn = (stm == white) ? P : p;
        
        // Remove the pawn
        popbit(bitboards[pawn], target);
        // Promote it
        setbit(bitboards[promoted], target);
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
    if (double_move) enpassant = (stm == white) ? target - 8 : target + 8;

    // Handle castling (update rook bitboard and own occupancy)
    if (castling_flag) {
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

    // Update move clocks
    hmclock++;
    // Equivalent to: if (stm == black) fmclock++;
    fmclock += black;

    // Switch side
    stm ^= 1;

    return 1;
}
