#include "attack_gen.hpp"

bboard pawn_att[2][64];
bboard knight_att[64];
bboard king_att[64];

bboard bishop_masks[64];
bboard rook_masks[64];

// Bishop relevant occupancy bit count
constexpr uint8_t bishop_relevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    6, 5, 5, 5, 5, 5, 5, 6,
};
// Rook relevant occupancy bit count
constexpr uint8_t rook_relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    12, 11, 11, 11, 11, 11, 11, 12,
};

constexpr bboard rook_magic_nums[64] = {
    0x8a80104000800020ULL,
    0x140002000100040ULL,
    0x2801880a0017001ULL,
    0x100081001000420ULL,
    0x200020010080420ULL,
    0x3001c0002010008ULL,
    0x8480008002000100ULL,
    0x2080088004402900ULL,
    0x800098204000ULL,
    0x2024401000200040ULL,
    0x100802000801000ULL,
    0x120800800801000ULL,
    0x208808088000400ULL,
    0x2802200800400ULL,
    0x2200800100020080ULL,
    0x801000060821100ULL,
    0x80044006422000ULL,
    0x100808020004000ULL,
    0x12108a0010204200ULL,
    0x140848010000802ULL,
    0x481828014002800ULL,
    0x8094004002004100ULL,
    0x4010040010010802ULL,
    0x20008806104ULL,
    0x100400080208000ULL,
    0x2040002120081000ULL,
    0x21200680100081ULL,
    0x20100080080080ULL,
    0x2000a00200410ULL,
    0x20080800400ULL,
    0x80088400100102ULL,
    0x80004600042881ULL,
    0x4040008040800020ULL,
    0x440003000200801ULL,
    0x4200011004500ULL,
    0x188020010100100ULL,
    0x14800401802800ULL,
    0x2080040080800200ULL,
    0x124080204001001ULL,
    0x200046502000484ULL,
    0x480400080088020ULL,
    0x1000422010034000ULL,
    0x30200100110040ULL,
    0x100021010009ULL,
    0x2002080100110004ULL,
    0x202008004008002ULL,
    0x20020004010100ULL,
    0x2048440040820001ULL,
    0x101002200408200ULL,
    0x40802000401080ULL,
    0x4008142004410100ULL,
    0x2060820c0120200ULL,
    0x1001004080100ULL,
    0x20c020080040080ULL,
    0x2935610830022400ULL,
    0x44440041009200ULL,
    0x280001040802101ULL,
    0x2100190040002085ULL,
    0x80c0084100102001ULL,
    0x4024081001000421ULL,
    0x20030a0244872ULL,
    0x12001008414402ULL,
    0x2006104900a0804ULL,
    0x1004081002402ULL,
};
constexpr bboard bishop_magic_nums[64] = {
    0x40040844404084ULL,
    0x2004208a004208ULL,
    0x10190041080202ULL,
    0x108060845042010ULL,
    0x581104180800210ULL,
    0x2112080446200010ULL,
    0x1080820820060210ULL,
    0x3c0808410220200ULL,
    0x4050404440404ULL,
    0x21001420088ULL,
    0x24d0080801082102ULL,
    0x1020a0a020400ULL,
    0x40308200402ULL,
    0x4011002100800ULL,
    0x401484104104005ULL,
    0x801010402020200ULL,
    0x400210c3880100ULL,
    0x404022024108200ULL,
    0x810018200204102ULL,
    0x4002801a02003ULL,
    0x85040820080400ULL,
    0x810102c808880400ULL,
    0xe900410884800ULL,
    0x8002020480840102ULL,
    0x220200865090201ULL,
    0x2010100a02021202ULL,
    0x152048408022401ULL,
    0x20080002081110ULL,
    0x4001001021004000ULL,
    0x800040400a011002ULL,
    0xe4004081011002ULL,
    0x1c004001012080ULL,
    0x8004200962a00220ULL,
    0x8422100208500202ULL,
    0x2000402200300c08ULL,
    0x8646020080080080ULL,
    0x80020a0200100808ULL,
    0x2010004880111000ULL,
    0x623000a080011400ULL,
    0x42008c0340209202ULL,
    0x209188240001000ULL,
    0x400408a884001800ULL,
    0x110400a6080400ULL,
    0x1840060a44020800ULL,
    0x90080104000041ULL,
    0x201011000808101ULL,
    0x1a2208080504f080ULL,
    0x8012020600211212ULL,
    0x500861011240000ULL,
    0x180806108200800ULL,
    0x4000020e01040044ULL,
    0x300000261044000aULL,
    0x802241102020002ULL,
    0x20906061210001ULL,
    0x5a84841004010310ULL,
    0x4010801011c04ULL,
    0xa010109502200ULL,
    0x4a02012000ULL,
    0x500201010098b028ULL,
    0x8040002811040900ULL,
    0x28000010020204ULL,
    0x6000020202d0240ULL,
    0x8918844842082200ULL,
    0x4010011029020020ULL,
};

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

bboard block_masks (square sq, bboard block) {
    bboard att = 0ULL;

    int8_t rank = getrank(sq);
    int8_t file = getfile(sq);

    // Edges are excluded
    for (int8_t r = rank - 1, f = file - 1; r > 0 && f > 0; r--, f--) {
        att |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (int8_t r = rank + 1, f = file - 1; r < 7 && f > 0; r++, f--) {
        att |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (int8_t r = rank - 1, f = file + 1; r > 0 && f < 7; r--, f++) {
        att |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
    }
    for (int8_t r = rank + 1, f = file + 1; r < 7 && f < 7; r++, f++) {
        att |= (1ULL << (r * 8 + f));
        if ((1ULL << (r * 8 + f)) & block) break;
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
            bb[sq][mi] = block_masks(sq, occ);
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
            bb[sq][mi] = block_masks(sq, occ);
        }
    }
}
