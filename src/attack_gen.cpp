#include "attack_gen.hpp"

bboard pawn_att[2][64];
bboard knight_att[64];
bboard king_att[64];

bboard bishop_masks[64];
bboard rook_masks[64];

bboard bishop_att[64][512];
bboard rook_att[64][4096];

void create_pawn_attacks (bboard (&bb)[2][64]) {
    // White pawn attacks
    for (square sq = a2; sq <= h7; sq++) {
        bb[white][sq] |= ((1ULL << sq << 7) & not_h_file);
        bb[white][sq] |= ((1ULL << sq << 9) & not_a_file);
    }

    // Black pawn attacks
    for (square sq = a2; sq <= h7; sq++) {
        bb[black][sq] |= ((1ULL << sq >> 7) & not_a_file);
        bb[black][sq] |= ((1ULL << sq >> 9) & not_h_file);
    }
}

void create_knight_attacks (bboard (&bb)[64]) {
    for (square sq = a1; sq <= h8; sq++) {
        bb[sq] |= ((1ULL << sq >> 17) & not_h_file);
        bb[sq] |= ((1ULL << sq >> 15) & not_a_file);
        bb[sq] |= ((1ULL << sq >> 10) & not_gh_files);
        bb[sq] |= ((1ULL << sq >> 6) & not_ab_files);

        bb[sq] |= ((1ULL << sq << 17) & not_a_file);
        bb[sq] |= ((1ULL << sq << 15) & not_h_file);
        bb[sq] |= ((1ULL << sq << 10) & not_ab_files);
        bb[sq] |= ((1ULL << sq << 6) & not_gh_files);
    }
}

void create_king_attacks (bboard (&bb)[64]) {
    for (square sq = a1; sq <= h8; sq++) {
        bb[sq] |= ((1ULL << sq >> 9) & not_h_file);
        bb[sq] |= ((1ULL << sq >> 8));
        bb[sq] |= ((1ULL << sq >> 7) & not_a_file);

        bb[sq] |= ((1ULL << sq >> 1) & not_h_file);
        bb[sq] |= ((1ULL << sq << 1) & not_a_file);

        bb[sq] |= ((1ULL << sq << 7) & not_h_file);
        bb[sq] |= ((1ULL << sq << 8));
        bb[sq] |= ((1ULL << sq << 9) & not_a_file);
    }
}

void create_bishop_masks (bboard (&bb)[64]) {
    for (square sq = a1; sq <= h8; sq++) {
        int8_t rank = getrank(sq);
        int8_t file = getfile(sq);

        // Edges are excluded
        for (int8_t r = rank - 1, f = file - 1; r > 0 && f > 0; r--, f--) {
            bb[sq] |= (1ULL << (r * 8 + f));
        }
        for (int8_t r = rank + 1, f = file - 1; r < 7 && f > 0; r++, f--) {
            bb[sq] |= (1ULL << (r * 8 + f));
        }
        for (int8_t r = rank - 1, f = file + 1; r > 0 && f < 7; r--, f++) {
            bb[sq] |= (1ULL << (r * 8 + f));
        }
        for (int8_t r = rank + 1, f = file + 1; r < 7 && f < 7; r++, f++) {
            bb[sq] |= (1ULL << (r * 8 + f));
        }
    }
}

void create_rook_masks (bboard (&bb)[64]) {
    for (square sq = a1; sq <= h8; sq++) {
        int8_t rank = getrank(sq);
        int8_t file = getfile(sq);

        // Edges are excluded
        for (int8_t r = rank - 1; r > 0; r--) {
            bb[sq] |= (1ULL << (r * 8 + file));
        }
        for (int8_t r = rank + 1; r < 7; r++) {
            bb[sq] |= (1ULL << (r * 8 + file));
        }
        for (int8_t f = file - 1; f > 0; f--) {
            bb[sq] |= (1ULL << (rank * 8 + f));
        }
        for (int8_t f = file + 1; f < 7; f++) {
            bb[sq] |= (1ULL << (rank * 8 + f));
        }
    }
}

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

void create_bishop_attacks (bboard (&bb)[64][512]) {
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
            bb[sq][mi] = create_bishop_table(sq, occ);
        }
    }
}

void create_rook_attacks (bboard (&bb)[64][4096]) {
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
            bb[sq][mi] = create_rook_table(sq, occ);
        }
    }
}
