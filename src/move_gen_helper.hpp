#pragma once

#include "attack_gen.hpp"
#include "bit_manip.hpp"
#include "board_rep.hpp"
#include "utils.hpp"

#include "debug.hpp"

typedef struct {
    move moves[256];
    counter index;
} Moves;

extern bboard blocker_tables[64][64];
extern bboard legal_mv_mask;
extern bboard pawn_mv_mask;
extern bboard pin_mask[64];

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
constexpr uint32_t en_passant_flag_mask = 0x400000;
constexpr uint32_t castling_flag_mask = 0x800000;

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

void create_blocker_tables (bboard (&bb)[64][64]);

// Handles pinned pieces
// pins: bitboard of the pieces pinned by the opponent
// pins_mask: mask to allow moving pinned piece only in pin's direction
__always_inline void find_pins(state side) {
    pins = 0ULL;
    square king_sq = getls1b(bitboards[(side == white) ? K : k]);

    // Get all possible opponents pinners (slider pieces)
    bboard bishop_like = (side == white) 
        ? (bitboards[b] | bitboards[q]) 
        : (bitboards[B] | bitboards[Q]);
    bboard rook_like = (side == white) 
        ? (bitboards[r] | bitboards[q]) 
        : (bitboards[R] | bitboards[Q]);
    bboard opponents = (bishop_like & get_bishop_att(king_sq, 0ULL)) | (rook_like & get_rook_att(king_sq, 0ULL));

    // For each potential pinner
    while (opponents) {
        square pinner_sq = getls1b(opponents);
        popbit(opponents, pinner_sq);

        // Check if it's in diagonal or line of king
        bboard between = blocker_tables[pinner_sq][king_sq];
        if (!between) continue;

        // Find pieces in between
        bboard blockers = between & occupancies[both];
        // If any && it's only one
        if (blockers && ((blockers & (blockers - 1ULL)) == 0ULL)) {
            square blocker_sq = getls1b(blockers);

            // Check if is a friendly piece
            if (getbit(occupancies[side], blocker_sq)) {
                pins |= (1ULL << blocker_sq);
                pin_mask[blocker_sq] = between | (1ULL << pinner_sq);
            }
        }

        bboard white_blocker_pawns = blockers & bitboards[P];
        bboard black_blocker_pawns = blockers & bitboards[p];
        if (countbits(blockers) == 2 && countbits(white_blocker_pawns) == 1 && countbits(black_blocker_pawns) == 1) {
            if (side == white) {
                square blocker_sq = getls1b(white_blocker_pawns);
                if (enpassant != no_sq && (pawn_att[white][blocker_sq] & (1ULL << enpassant)) && (1ULL << (enpassant - 8)) & black_blocker_pawns) {
                    enpassant = no_sq;
                }
            } else {
                square blocker_sq = getls1b(black_blocker_pawns);
                if (enpassant != no_sq && (pawn_att[black][blocker_sq] & (1ULL << enpassant)) && ((1ULL << (enpassant + 8)) & white_blocker_pawns)) {
                    enpassant = no_sq;
                }
            }
        }
    }
}

// Generate a bitboard of pieces (of the given side) that are checking the king
__always_inline void find_checkers (state side) {
    checkers = 0ULL;
    square sq = getls1b(bitboards[(side == white) ? k : K]);

    // White pawns (attackers)
    if (side == white) {
        checkers |= (pawn_att[black][sq] & bitboards[P]);
    }

    // Black pawns (attackers)
    if (side == black) {
        checkers |= (pawn_att[white][sq] & bitboards[p]);
    }

    checkers |= get_rook_att(sq, occupancies[both]) & ((side == white) ? bitboards[R] : bitboards[r]);

    checkers |= knight_att[sq] & ((side == white) ? bitboards[N] : bitboards[n]);

    checkers |= get_bishop_att(sq, occupancies[both]) & ((side == white) ? bitboards[B] : bitboards[b]);

    checkers |= get_queen_att(sq, occupancies[both]) & ((side == white) ? bitboards[Q] : bitboards[q]);
}

// Check if the current given square is attacked by the current given side
__always_inline bool is_sq_attacked (state side, square sq) {
    // Attacked by white pawns
    if ((side == white) && (pawn_att[black][sq] & bitboards[P])) {
        return true;
    }

    // Attacked by black pawns
    if ((side == black) && (pawn_att[white][sq] & bitboards[p])) {
        return true;
    }

    // Attacked by rooks
    if (get_rook_att(sq, occupancies[both]) & ((side == white) ? bitboards[R] : bitboards[r])) {
        return true;
    }

    // Attacked by knights
    if (knight_att[sq] & ((side == white) ? bitboards[N] : bitboards[n])) {
        return true;
    }

    // Attacked by bishops
    if (get_bishop_att(sq, occupancies[both]) & ((side == white) ? bitboards[B] : bitboards[b])) {
        return true;
    }

    // Attacked by king
    if (king_att[sq] & ((side == white) ? bitboards[K] : bitboards[k])) {
        return true;
    }

    // Attacked by queen
    if (get_queen_att(sq, occupancies[both]) & ((side == white) ? bitboards[Q] : bitboards[q])) {
        return true;
    }

    return false;
}

__always_inline move encode_move (square source, square target, piece curr_piece, 
    piece promoted, flag capture, flag doublepush, flag enpassant, flag castling) {
    return (source         | 
        (target << 6)      |
        (curr_piece << 12) |
        (promoted << 16)   |
        (capture << 20)    | 
        (doublepush << 21) |
        (enpassant << 22)  |
        (castling << 23)
    );
}

__always_inline void add_move (Moves &move_list, move mv) {
    move_list.moves[move_list.index] = mv;
    move_list.index++;
}

__always_inline void generate_moves_white_pawn(Moves &move_list) {
    piece curr_piece = P;
    square source_square;
    square target_square;
    bboard bitboard = bitboards[curr_piece];
    bboard attacks;

    while (bitboard) {
        // Init source square
        source_square = getls1b(bitboard);

        // Init target square
        target_square = source_square + 8;

        bool pinned = getbit(pins, source_square);

        // Generate quiet pawn moves
        if ((target_square >= a1) && !getbit(occupancies[both], target_square)) {
            // Pawn promotion
            if (a7 <= source_square && source_square <= h7 && getbit(pawn_mv_mask, target_square) && (!pinned || getbit(pin_mask[source_square], target_square))) {
                add_move(move_list, encode_move(source_square, target_square, curr_piece, Q, 0, 0, 0, 0));
                add_move(move_list, encode_move(source_square, target_square, curr_piece, R, 0, 0, 0, 0));
                add_move(move_list, encode_move(source_square, target_square, curr_piece, B, 0, 0, 0, 0));
                add_move(move_list, encode_move(source_square, target_square, curr_piece, N, 0, 0, 0, 0));
            }

            else {
                // One square pawn move
                if (getbit(pawn_mv_mask, target_square) && (!pinned || getbit(pin_mask[source_square], target_square))) {
                    add_move(move_list, encode_move(source_square, target_square, curr_piece, 0, 0, 0, 0, 0));
                }

                // Two square pawn move
                if ((a2 <= source_square && source_square <= h2) && !getbit(occupancies[both], target_square + 8) && getbit(pawn_mv_mask, target_square + 8) && (!pinned || getbit(pin_mask[source_square], target_square + 8))) {
                    add_move(move_list, encode_move(source_square, target_square + 8, curr_piece, 0, 0, 1, 0, 0));
                }
            }
        }

        // Init pawn attacks bitboard
        attacks = pawn_att[white][source_square] & occupancies[black];
        if (getbit(pins, source_square)) attacks &= pin_mask[source_square];
        attacks &= pawn_mv_mask;

        // Generate pawn captures
        while (attacks) {
            // Init target square
            target_square = getls1b(attacks);

            // Pawn capture promotion
            if (a7 <= source_square && source_square <= h7) {
                add_move(move_list, encode_move(source_square, target_square, curr_piece, Q, 1, 0, 0, 0));
                add_move(move_list, encode_move(source_square, target_square, curr_piece, R, 1, 0, 0, 0));
                add_move(move_list, encode_move(source_square, target_square, curr_piece, B, 1, 0, 0, 0));
                add_move(move_list, encode_move(source_square, target_square, curr_piece, N, 1, 0, 0, 0));
            }

            else {
                // Pawn capture
                add_move(move_list, encode_move(source_square, target_square, curr_piece, 0, 1, 0, 0, 0));
            }   

            // Pop least significant bit from attacks bitboard
            popbit(attacks, target_square);
        }

        // Generate en passant capture
        if (enpassant != no_sq && getbit(pawn_mv_mask, enpassant) && (!pinned || getbit(pin_mask[source_square], enpassant))) {
            // Create en passant attack bitboard
            bboard en_passant_attacks = pawn_att[white][source_square] & (1ULL << enpassant);

            // En passant capture available for this pawn
            if (en_passant_attacks) {
                // Init en passant target square
                target_square = getls1b(en_passant_attacks);

                add_move(move_list, encode_move(source_square, target_square, curr_piece, 0, 1, 0, 1, 0));
            }
        }

        // Pop least significant bit from bitboard copy
        popbit(bitboard, source_square);
    }
}

__always_inline void generate_moves_black_pawn(Moves &move_list) {
    piece curr_piece = p;
    square source_square;
    square target_square;
    bboard bitboard = bitboards[curr_piece];
    bboard attacks;

    while (bitboard) {
        // Init source square
        source_square = getls1b(bitboard);
        // Init target square
        target_square = source_square - 8;

        bool pinned = getbit(pins, source_square);

        // Generate quiet pawn moves
        if ((target_square <= h8) && !getbit(occupancies[both], target_square)) {
            // Pawn promotion
            if (a2 <= source_square && source_square <= h2 && getbit(pawn_mv_mask, target_square) && (!pinned || getbit(pin_mask[source_square], target_square))) {
                add_move(move_list, encode_move(source_square, target_square, curr_piece, q, 0, 0, 0, 0));
                add_move(move_list, encode_move(source_square, target_square, curr_piece, r, 0, 0, 0, 0));
                add_move(move_list, encode_move(source_square, target_square, curr_piece, b, 0, 0, 0, 0));
                add_move(move_list, encode_move(source_square, target_square, curr_piece, n, 0, 0, 0, 0));
            }

            else {
                // One square pawn move
                if (getbit(pawn_mv_mask, target_square) && (!pinned || getbit(pin_mask[source_square], target_square))) {
                    add_move(move_list, encode_move(source_square, target_square, curr_piece, 0, 0, 0, 0, 0));
                }

                // Two square pawn move
                if ((a7 <= source_square && source_square <= h7) && !getbit(occupancies[both], target_square - 8) && getbit(pawn_mv_mask, target_square - 8) && (!pinned || getbit(pin_mask[source_square], target_square - 8))) {
                    add_move(move_list, encode_move(source_square, target_square - 8, curr_piece, 0, 0, 1, 0, 0));
                }
            }
        }

        // Init pawn attacks bitboard
        attacks = pawn_att[black][source_square] & occupancies[white];
        if (getbit(pins, source_square)) attacks &= pin_mask[source_square];
        attacks &= pawn_mv_mask;

        // Generate pawn captures
        while (attacks) {
            // Init target square
            target_square = getls1b(attacks);

            // Pawn capture promotion
            if (a2 <= source_square && source_square <= h2) {
                add_move(move_list, encode_move(source_square, target_square, curr_piece, q, 1, 0, 0, 0));
                add_move(move_list, encode_move(source_square, target_square, curr_piece, r, 1, 0, 0, 0));
                add_move(move_list, encode_move(source_square, target_square, curr_piece, b, 1, 0, 0, 0));
                add_move(move_list, encode_move(source_square, target_square, curr_piece, n, 1, 0, 0, 0));
            }

            else {
                // Pawn capture
                add_move(move_list, encode_move(source_square, target_square, curr_piece, 0, 1, 0, 0, 0));
            }

            // Pop least significant bit from attacks bitboard
            popbit(attacks, target_square);
        }

        // Generate en passant capture
        if (enpassant != no_sq && getbit(pawn_mv_mask, enpassant) && (!pinned || getbit(pin_mask[source_square], enpassant))) {
            // Create en passant attack bitboard
            bboard en_passant_attacks = pawn_att[black][source_square] & (1ULL << enpassant);

            // En passant capture available for this pawn
            if (en_passant_attacks) {
                // Init en passant target square
                target_square = getls1b(en_passant_attacks);

                add_move(move_list, encode_move(source_square, target_square, curr_piece, 0, 1, 0, 1, 0));
            }
        }

        // Pop least significant bit from bitboard copy
        popbit(bitboard, source_square);
    }
}

__always_inline void generate_moves_rook (Moves &move_list, state side) {
    square source_sq;
    square target_sq;
    piece curr_piece = (side == white) ? R : r;
    bboard bitboard = bitboards[curr_piece];
    bboard attacks;

    while (bitboard) {
        // Init source square
        source_sq = getls1b(bitboard);

        // Init piece attacks
        attacks = get_rook_att(source_sq, occupancies[both]) & (~occupancies[side]);
        if (getbit(pins, source_sq)) attacks &= pin_mask[source_sq];
        attacks &= legal_mv_mask;

        // Loop over target squares available
        while (attacks) {
            // Init target square
            target_sq = getls1b(attacks);

            if (!getbit(occupancies[!side], target_sq)) {
                // Quiet move
                add_move(move_list, encode_move(source_sq, target_sq, curr_piece, 0, 0, 0, 0, 0));
            } else {
                // Capture move
                add_move(move_list, encode_move(source_sq, target_sq, curr_piece, 0, 1, 0, 0, 0));
            }

            // Pop least significant bit from attacks bitboard
            popbit(attacks, target_sq);
        }

        // Pop least significant bit from bitboard copy
        popbit(bitboard, source_sq);
    }
}

__always_inline void generate_moves_knight (Moves &move_list, state side) {
    square source_sq;
    square target_sq;
    piece curr_piece = (side == white) ? N : n;
    bboard bitboard = bitboards[curr_piece];
    bboard attacks;

    while (bitboard) {
        source_sq = getls1b(bitboard);
        // Get attacks that do not land on same side's pieces
        attacks = knight_att[source_sq] & (~occupancies[side]);
        if (getbit(pins, source_sq)) attacks &= pin_mask[source_sq];
        attacks &= legal_mv_mask;

        while (attacks) {
            target_sq = getls1b(attacks);

            if (!getbit(occupancies[!side], target_sq)) {
                add_move(move_list, encode_move(source_sq, target_sq, curr_piece, 0, 0, 0, 0, 0));
            } else {
                add_move(move_list, encode_move(source_sq, target_sq, curr_piece, 0, 1, 0, 0, 0));
            }

            popbit(attacks, target_sq);
        }

        popbit(bitboard, source_sq);
    }
}

__always_inline void generate_moves_bishop (Moves &move_list, state side) {
    square source_sq;
    square target_sq;
    piece curr_piece = (side == white) ? B : b;
    bboard bitboard = bitboards[curr_piece];
    bboard attacks;

    while (bitboard) {
        source_sq = getls1b(bitboard);
        // Get attacks that do not land on same side's pieces
        attacks = get_bishop_att(source_sq, occupancies[both]) & (~occupancies[side]);
        if (getbit(pins, source_sq)) attacks &= pin_mask[source_sq];
        attacks &= legal_mv_mask;

        while (attacks) {
            target_sq = getls1b(attacks);

            if (!getbit(occupancies[!side], target_sq)) {
                add_move(move_list, encode_move(source_sq, target_sq, curr_piece, 0, 0, 0, 0, 0));
            } else {
                add_move(move_list, encode_move(source_sq, target_sq, curr_piece, 0, 1, 0, 0, 0));
            }

            popbit(attacks, target_sq);
        }

        popbit(bitboard, source_sq);
    }
}

__always_inline void generate_white_castling (Moves &move_list) {
    // King side castling is available
    if (castling & wk) {
        // Make sure squares between king and rook are empty
        if(!getbit(occupancies[both], f1) && !getbit(occupancies[both], g1)) {
            // Make sure king's and F1 squares are not under attack
            if(!is_sq_attacked(black, e1) && !is_sq_attacked(black, f1) && !is_sq_attacked(black, g1)) {
                add_move(move_list, encode_move(e1, g1, K, 0, 0, 0, 0, 1));
            }
        }
    }

    // Queen side castling is available
    if (castling & wq) {
        // Make sure squares between king and rook are empty
        if(!getbit(occupancies[both], d1) && !getbit(occupancies[both], c1) && !getbit(occupancies[both], b1)) {
            // Make sure king's and D1 squares are not under attack
            if(!is_sq_attacked(black, e1) && !is_sq_attacked(black, d1) && !is_sq_attacked(black, c1)) {
                add_move(move_list, encode_move(e1, c1, K, 0, 0, 0, 0, 1));
            }
        }
    }
}

__always_inline void generate_black_castling (Moves &move_list) {
     // King side castling is available
    if (castling & bk) {
        // Make sure squares between king and rook are empty
        if(!getbit(occupancies[both], f8) && !getbit(occupancies[both], g8)) {
            // Make sure king's and F8 squares are not under attack
            if(!is_sq_attacked(white, e8) && !is_sq_attacked(white, f8) && !is_sq_attacked(white, g8)) {
                add_move(move_list, encode_move(e8, g8, k, 0, 0, 0, 0, 1));
            }
        }
    }

    // Queen side castling is available
    if (castling & bq) {
        // Make sure squares between king and rook are empty
        if(!getbit(occupancies[both], d8) && !getbit(occupancies[both], c8) && !getbit(occupancies[both], b8)) {
            // Make sure king's and D8 squares are not under attack
            if(!is_sq_attacked(white, e8) && !is_sq_attacked(white, d8) && !is_sq_attacked(white, c8)) {
                add_move(move_list, encode_move(e8, c8, k, 0, 0, 0, 0, 1));
            }
        }
    }
}

__always_inline void generate_moves_king (Moves &move_list, state side) {
    square source_sq;
    square target_sq;
    piece curr_piece = (side == white) ? K : k;
    bboard bitboard = bitboards[curr_piece];
    bboard attacks;

    if (side == white) {
        generate_white_castling(move_list);
    } else {
        generate_black_castling(move_list);
    }

    while (bitboard) {
        // Init source square
        source_sq = getls1b(bitboard);

        // Init piece attacks
        attacks = king_att[source_sq] & (~occupancies[side]);

        // Loop over target squares available
        while (attacks) {
            // Init target square
            target_sq = getls1b(attacks);

            // Remove king from the bitboard to avoid ghost blocking itself
            popbit(bitboards[curr_piece], source_sq);
            popbit(occupancies[stm], source_sq);
            popbit(occupancies[both], source_sq);
            if (is_sq_attacked(!side, target_sq)) {
                setbit(bitboards[curr_piece], source_sq);
                setbit(occupancies[stm], source_sq);
                setbit(occupancies[both], source_sq);
                popbit(attacks, target_sq);
                continue;
            }
            setbit(bitboards[curr_piece], source_sq);
            setbit(occupancies[stm], source_sq);
            setbit(occupancies[both], source_sq);

            if (!getbit(occupancies[!side], target_sq)) {
                // Quiet move
                add_move(move_list, encode_move(source_sq, target_sq, curr_piece, 0, 0, 0, 0, 0));
            } else {
                // Capture move
                add_move(move_list, encode_move(source_sq, target_sq, curr_piece, 0, 1, 0, 0, 0));
            }

            // Pop least significant bit from attacks bitboard
            popbit(attacks, target_sq);
        }

        // Pop least significant bit from bitboard copy
        popbit(bitboard, source_sq);
    }
}

__always_inline void generate_moves_queen (Moves &move_list, state side) {
    square source_sq;
    square target_sq;
    piece curr_piece = (side == white) ? Q : q;
    bboard bitboard = bitboards[curr_piece];
    bboard attacks;

    while (bitboard) {
        // Init source square
        source_sq = getls1b(bitboard);

        // Init piece attacks
        attacks = get_queen_att(source_sq, occupancies[both]) & (~occupancies[side]);
        if (getbit(pins, source_sq)) attacks &= pin_mask[source_sq];
        attacks &= legal_mv_mask;

        // Loop over target squares available
        while (attacks) {
            // Init target square
            target_sq = getls1b(attacks);

            if (!getbit(occupancies[!side], target_sq)) {
                // Quiet move
                add_move(move_list, encode_move(source_sq, target_sq, curr_piece, 0, 0, 0, 0, 0));
            } else {
                // Capture move
                add_move(move_list, encode_move(source_sq, target_sq, curr_piece, 0, 1, 0, 0, 0));
            }

            // Pop least significant bit from attacks bitboard
            popbit(attacks, target_sq);
        }

        // Pop least significant bit from bitboard copy
        popbit(bitboard, source_sq);
    }
}
