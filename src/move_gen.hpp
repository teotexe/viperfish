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
    move_list.nextindex = 0;

    find_checkers(!pos.stm);

    pawn_mv_mask = 0ULL;

    if (pos.stm == white) {
        if (countbits(checkers) > 1) {
            generate_moves_king(move_list, white);
            return;
        }

        if (countbits(checkers) == 1) {
            Type::square king_sq = getls1b(pos.bitboards[K]);
            Type::square checker_sq = getls1b(checkers);
            legal_mv_mask = blocker_tables[king_sq][checker_sq] | checkers;
            if (pos.enpassant != no_sq && (1ULL << (pos.enpassant - 8)) & checkers) pawn_mv_mask |= (1ULL << pos.enpassant);
        } else {
            legal_mv_mask = 0xFFFFFFFFFFFFFFFFULL;
        }
        pawn_mv_mask |= legal_mv_mask;

        // Compute pins and pins_mask
        find_pins(pos.stm);

        for (Type::piece curr_piece = P; curr_piece <= Q; curr_piece++) {
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
            Type::square king_sq = getls1b(pos.bitboards[k]);
            Type::square checker_sq = getls1b(checkers);
            legal_mv_mask = blocker_tables[king_sq][checker_sq] | checkers;
            if (pos.enpassant != no_sq && (1ULL << (pos.enpassant + 8)) & checkers) pawn_mv_mask |= (1ULL << pos.enpassant);
        } else {
            legal_mv_mask = 0xFFFFFFFFFFFFFFFFULL;
        }
        pawn_mv_mask |= legal_mv_mask;

        // Compute pins and pins_mask
        find_pins(pos.stm);

        for (Type::piece curr_piece = p; curr_piece <= q; curr_piece++) {
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
    Type::square source = get_mv_src(move);
    Type::square target = get_mv_trgt(move);
    Type::piece curr_piece = get_mv_piece(move);
    Type::piece promoted = get_mv_prmtd(move);
    bool capture = get_mv_cptr(move);
    bool double_move = get_mv_dblpsh(move);
    bool en_passant_move = get_mv_enpsnt(move);
    bool castling_flag = get_mv_cstlng(move);

    // Refs to occupancies bitboards
    Type::bboard *own_occ = &pos.occupancies[pos.stm];
    Type::bboard *opp_occ = &pos.occupancies[!pos.stm];

    // Move the piece
    popbit(pos.bitboards[curr_piece], source);
    setbit(pos.bitboards[curr_piece], target);

    // Update own occupancy
    popbit(*own_occ, source);
    setbit(*own_occ, target);

    // Handle captures (including normal capture on target)
    if (capture) {
        int cap_sq = target;
        // En passant captures use different square
        if (en_passant_move) cap_sq = (pos.stm == white) ? (target - 8) : (target + 8);

        // Remove from opponent occupancies
        popbit(*opp_occ, cap_sq);

        // Find the opponent's piece on the capture square and remove it

        // Loop over opposite side's pieces
        Type::piece start = (pos.stm == white) ? p : P;
        Type::piece end   = (pos.stm == white) ? q : Q;
        for (Type::piece pc = start; pc <= end; pc++) {
            if (getbit(pos.bitboards[pc], cap_sq)) {
                popbit(pos.bitboards[pc], cap_sq);
                break;
            }
        }
    }

    // Handle promotion
    if (promoted) {
        Type::piece pawn = (pos.stm == white) ? P : p;
        
        // Remove the pawn
        popbit(pos.bitboards[pawn], target);
        // Promote it
        setbit(pos.bitboards[promoted], target);
    }

    // Reset en passant and set if double pawn push
    pos.enpassant = no_sq;
    if (double_move) pos.enpassant = (pos.stm == white) ? target - 8 : target + 8;

    // Handle castling (update rook bitboard and own occupancy)
    if (castling_flag) {
        switch (target) {
            case g1:
                popbit(pos.bitboards[R], h1);
                setbit(pos.bitboards[R], f1);
                popbit(*own_occ, h1);
                setbit(*own_occ, f1);
                break;
            case c1:
                popbit(pos.bitboards[R], a1);
                setbit(pos.bitboards[R], d1);
                popbit(*own_occ, a1);
                setbit(*own_occ, d1);
                break;
            case g8:
                popbit(pos.bitboards[r], h8);
                setbit(pos.bitboards[r], f8);
                popbit(*own_occ, h8);
                setbit(*own_occ, f8);
                break;
            case c8:
                popbit(pos.bitboards[r], a8);
                setbit(pos.bitboards[r], d8);
                popbit(*own_occ, a8);
                setbit(*own_occ, d8);
                break;
            default:
                break;
        }
    }

    // Update castling rights
    pos.castling &= update_castling_rights[source];
    pos.castling &= update_castling_rights[target];

    // Update combined occupancy
    pos.occupancies[both] = pos.occupancies[white] | pos.occupancies[black];

    // Update move clocks
    pos.hmclock++;
    // Equivalent to: if (pos.stm == black) fmclock++;
    pos.fmclock += black;

    // Switch side
    pos.stm ^= 1;

    return 1;
}
