#include "move_gen_helper.hpp"

bboard blocker_tables[64][64];
bboard legal_mv_mask;

void create_blocker_tables (bboard (&bb)[64][64]) {
    for (square sq = a1; sq <= h8; sq++) {
        int8_t rank = getrank(sq);
        int8_t file = getfile(sq);

        // Bishop blocker moves
        bboard bits_inbetween = 0ULL;
        for (int8_t r = rank - 2, f = file - 2; r >= 0 && f >= 0; r--, f--) {
            bits_inbetween |= (1ULL << ((r + 1) * 8 + (f + 1)));
            bb[sq][r * 8 + f] |= bits_inbetween;
        }

        bits_inbetween = 0ULL;
        for (int8_t r = rank - 2, f = file + 2; r >= 0 && f <= 7; r--, f++) {
            bits_inbetween |= (1ULL << ((r + 1) * 8 + (f - 1)));
            bb[sq][r * 8 + f] |= bits_inbetween;
        }

        bits_inbetween = 0ULL;
        for (int8_t r = rank + 2, f = file - 2; r <= 7 && f >= 0; r++, f--) {
            bits_inbetween |= (1ULL << ((r - 1) * 8 + (f + 1)));
            bb[sq][r * 8 + f] |= bits_inbetween;
        }

        bits_inbetween = 0ULL;
        for (int8_t r = rank + 2, f = file + 2; r <= 7 && f <= 7; r++, f++) {
            bits_inbetween |= (1ULL << ((r - 1) * 8 + (f - 1)));
            bb[sq][r * 8 + f] |= bits_inbetween;
        }

        // Rook blocker moves
        bits_inbetween = 0ULL;
        for (int8_t r = rank - 2; r >= 0; r--){
            bits_inbetween |= (1ULL << ((r + 1) * 8 + file));
            bb[sq][r * 8 + file] |= bits_inbetween;
        }
        
        bits_inbetween = 0ULL;
        for (int8_t f = file - 2; f >= 0; f--){
            bits_inbetween |= (1ULL << (rank * 8 + (f + 1)));
            bb[sq][rank * 8 + f] |= bits_inbetween;
        }

        bits_inbetween = 0ULL;
        for (int8_t r = rank + 2; r <= 7; r++){
            bits_inbetween |= (1ULL << ((r - 1) * 8 + file));
            bb[sq][r * 8 + file] |= bits_inbetween;
        }
        
        bits_inbetween = 0ULL;
        for (int8_t f = file + 2; f <= 7; f++){
            bits_inbetween |= (1ULL << (rank * 8 + (f - 1)));
            bb[sq][rank * 8 + f] |= bits_inbetween;
        }
    }
}
