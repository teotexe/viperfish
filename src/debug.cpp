#include "debug.hpp"

void print_bboard (bboard bb) {
    for (int8_t rank = 0; rank < 8; rank++) {
        for (int8_t file = 0; file < 8; file++) {
            std::cout << getbit(bb, rank * 8 + file);
        }
        std::cout << "\n";
    }

    std::cout << "\n\n";
}

void print_board () {
    for (int8_t rank = 7; rank >= 0; rank--) {
        for (int8_t file = 0; file <= 7; file++) {
            square sq = rank * 8 + file;
            if (!getbit(occupancies[both], sq)) {
                std::cout << ".";
                continue;
            }

            piece pc;
            for (piece candidate = P; candidate <= q; candidate++) {
                if (getbit(bitboards[candidate], sq)) {
                    pc = candidate;
                    break;
                }
            }

            std::cout << char_pieces[pc];
        }
        std::cout << "\n";
    }

    std::cout << "\n\n";
}
