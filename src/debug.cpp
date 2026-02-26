#include "debug.hpp"

void print_bboard (Type::bboard bb) {
    for (int8_t rank = 7; rank >= 0; rank--) {
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
            Type::square sq = rank * 8 + file;
            if (!getbit(pos.occupancies[both], sq)) {
                std::cout << ".";
                continue;
            }

            Type::piece pc;
            for (Type::piece candidate = P; candidate <= q; candidate++) {
                if (getbit(pos.bitboards[candidate], sq)) {
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
