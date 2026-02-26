#include "attack_gen.hpp"

bboard Attacks::pawn_att[2][64];
bboard Attacks::knight_att[64];
bboard Attacks::king_att[64];

bboard Attacks::bishop_masks[64];
bboard Attacks::rook_masks[64];

bboard Attacks::bishop_att[64][512];
bboard Attacks::rook_att[64][4096];

bboard create_bishop_table (square sq, bboard block) {
    bboard att = 0ULL;

    int8_t rank = getrank(sq);
    int8_t file = getfile(sq);

    for (int8_t r = rank - 1, f = file - 1; r >= 0 && f >= 0; r--, f--) {
        att |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (int8_t r = rank + 1, f = file - 1; r <= 7 && f >= 0; r++, f--) {
        att |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (int8_t r = rank - 1, f = file + 1; r >= 0 && f <= 7; r--, f++) {
        att |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (int8_t r = rank + 1, f = file + 1; r <= 7 && f <= 7; r++, f++) {
        att |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }

    return att;
}

bboard create_rook_table (square sq, bboard block) {
    bboard att = 0ULL;

    int8_t rank = getrank(sq);
    int8_t file = getfile(sq);

    for (int8_t r = rank - 1; r >= 0; r--) {
        att |= (1ULL << (r * 8 + file));
        if ((1ULL << (r * 8 + file)) & block) break;
    }
    for (int8_t r = rank + 1; r <= 7; r++) {
        att |= (1ULL << (r * 8 + file));
        if ((1ULL << (r * 8 + file)) & block) break;
    }
    for (int8_t f = file - 1; f >= 0; f--) {
        att |= (1ULL << (rank * 8 + f));
        if ((1ULL << (rank * 8 + f)) & block) break;
    }
    for (int8_t f = file + 1; f <= 7; f++) {
        att |= (1ULL << (rank * 8 + f));
        if ((1ULL << (rank * 8 + f)) & block) break;
    }

    return att;
}

bboard set_occupancy (uint16_t index, bboard mask) {
    bboard occupancy = 0ULL;
    uint8_t bits = countbits(mask);

    // Project the binary representation of index onto the occupancy bitboard
    for (uint16_t i = 0; i < bits; i++) {
        square sq = getls1b(mask);
        popbit(mask, sq);

        if (index & (1 << i)) {
            occupancy |= (1ULL << sq);
        }
    }

    return occupancy;
}

void Attacks::init_pawn_attacks () {
    // White pawn attacks
    for (square sq = a1; sq <= h8; sq++) {
        pawn_att[white][sq] |= ((1ULL << sq << 7) & not_h_file);
        pawn_att[white][sq] |= ((1ULL << sq << 9) & not_a_file);
    }

    // Black pawn attacks
    for (square sq = a1; sq <= h8; sq++) {
        pawn_att[black][sq] |= ((1ULL << sq >> 7) & not_a_file);
        pawn_att[black][sq] |= ((1ULL << sq >> 9) & not_h_file);
    }
}

void Attacks::init_knight_attacks () {
    for (square sq = a1; sq <= h8; sq++) {
        knight_att[sq] |= ((1ULL << sq >> 17) & not_h_file);
        knight_att[sq] |= ((1ULL << sq >> 15) & not_a_file);
        knight_att[sq] |= ((1ULL << sq >> 10) & not_gh_files);
        knight_att[sq] |= ((1ULL << sq >> 6) & not_ab_files);

        knight_att[sq] |= ((1ULL << sq << 17) & not_a_file);
        knight_att[sq] |= ((1ULL << sq << 15) & not_h_file);
        knight_att[sq] |= ((1ULL << sq << 10) & not_ab_files);
        knight_att[sq] |= ((1ULL << sq << 6) & not_gh_files);
    }
}

void Attacks::init_king_attacks () {
    for (square sq = a1; sq <= h8; sq++) {
        king_att[sq] |= ((1ULL << sq >> 9) & not_h_file);
        king_att[sq] |= ((1ULL << sq >> 8));
        king_att[sq] |= ((1ULL << sq >> 7) & not_a_file);

        king_att[sq] |= ((1ULL << sq >> 1) & not_h_file);
        king_att[sq] |= ((1ULL << sq << 1) & not_a_file);

        king_att[sq] |= ((1ULL << sq << 7) & not_h_file);
        king_att[sq] |= ((1ULL << sq << 8));
        king_att[sq] |= ((1ULL << sq << 9) & not_a_file);
    }
}

void Attacks::init_bishop_masks () {
    for (square sq = a1; sq <= h8; sq++) {
        int8_t rank = getrank(sq);
        int8_t file = getfile(sq);

        // Edges are excluded
        for (int8_t r = rank - 1, f = file - 1; r > 0 && f > 0; r--, f--) {
            bishop_masks[sq] |= (1ULL << (r * 8 + f));
        }
        for (int8_t r = rank + 1, f = file - 1; r < 7 && f > 0; r++, f--) {
            bishop_masks[sq] |= (1ULL << (r * 8 + f));
        }
        for (int8_t r = rank - 1, f = file + 1; r > 0 && f < 7; r--, f++) {
            bishop_masks[sq] |= (1ULL << (r * 8 + f));
        }
        for (int8_t r = rank + 1, f = file + 1; r < 7 && f < 7; r++, f++) {
            bishop_masks[sq] |= (1ULL << (r * 8 + f));
        }
    }
}

void Attacks::init_rook_masks () {
    for (square sq = a1; sq <= h8; sq++) {
        int8_t rank = getrank(sq);
        int8_t file = getfile(sq);

        // Edges are excluded
        for (int8_t r = rank - 1; r > 0; r--) {
            rook_masks[sq] |= (1ULL << (r * 8 + file));
        }
        for (int8_t r = rank + 1; r < 7; r++) {
            rook_masks[sq] |= (1ULL << (r * 8 + file));
        }
        for (int8_t f = file - 1; f > 0; f--) {
            rook_masks[sq] |= (1ULL << (rank * 8 + f));
        }
        for (int8_t f = file + 1; f < 7; f++) {
            rook_masks[sq] |= (1ULL << (rank * 8 + f));
        }
    }
}

void Attacks::init_bishop_attacks () {
    // Occupancy
    bboard occ;

    for (square sq = a1; sq <= h8; sq++) {
        // Number of different positions the blockers can be in
        uint16_t possibilities = (1 << countbits(bishop_masks[sq]));

        // Loop over all possibilities and calculate the tables
        for (uint16_t pos = 0; pos < possibilities; pos++) {
            occ = set_occupancy(pos, bishop_masks[sq]);

            // Magic index
            uint16_t mi;
            mi = (occ * bishop_magic_nums[sq]) >> (64 - bishop_relevant_bits[sq]);
            bishop_att[sq][mi] = create_bishop_table(sq, occ);
        }
    }
}

void Attacks::init_rook_attacks () {
    // Occupancy
    bboard occ;

    for (square sq = a1; sq <= h8; sq++) {
        // Number of different positions the blockers can be in
        uint16_t possibilities = (1 << countbits(rook_masks[sq]));

        // Loop over all possibilities and calculate the tables
        for (uint16_t pos = 0; pos < possibilities; pos++){
            occ = set_occupancy(pos, rook_masks[sq]);

            // Magic index
            uint16_t mi;
            mi = (occ * rook_magic_nums[sq]) >> (64 - rook_relevant_bits[sq]);
            rook_att[sq][mi] = create_rook_table(sq, occ);
        }
    }
}

void Attacks::init () {
    init_pawn_attacks();
    init_knight_attacks();
    init_king_attacks();
    init_bishop_masks();
    init_rook_masks();
    init_bishop_attacks();
    init_rook_attacks();
}
