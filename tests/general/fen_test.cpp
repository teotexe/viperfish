#include "fen_test.hpp"

bool run_fen_tests () {
    std::cout << "[?] FEN PARSING TESTS RUNNING\n"; 
    bool fail = false;

    parse_fen(starting_pos);
    bboard expected_starting[12] = {
        0x000000000000FF00ULL,
        0x0000000000000081ULL,
        0x0000000000000042ULL,
        0x0000000000000024ULL,
        0x0000000000000010ULL,
        0x0000000000000008ULL,
        0x00FF000000000000ULL,
        0x8100000000000000ULL,
        0x4200000000000000ULL,
        0x2400000000000000ULL,
        0x1000000000000000ULL,
        0x0800000000000000ULL,
    };

    for (piece pc = P; pc <= q; pc++) {
        if (bitboards[pc] == expected_starting[pc]) {
            std::cout << "[PASS]\n";
        } else {
            std::cout << "[FAIL]\n";
            fail = true;
        }
    }

    if (stm == white && castling == 15 && enpassant == no_sq) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }

    if (hmclock == 0 && fmclock == 1) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }

    parse_fen(tricky_pos);
    bboard expected_tricky[12] = {
        0x000000081000E700ULL,
        0x0000000000000081ULL,
        0x0000001000040000ULL,
        0x0000000000001800ULL,
        0x0000000000000010ULL,
        0x0000000000200000ULL,
        0x002D500002800000ULL,
        0x8100000000000000ULL,
        0x0000220000000000ULL,
        0x0040010000000000ULL,
        0x1000000000000000ULL,
        0x0010000000000000ULL,
    };

    for (piece pc = P; pc <= q; pc++) {
        if (bitboards[pc] == expected_tricky[pc]) {
            std::cout << "[PASS]\n";
        } else {
            std::cout << "[FAIL]\n";
            std::cout << "expected: " << expected_tricky[pc] << "\n";
            std::cout << "actual: " << bitboards[pc] << "\n";
            fail = true;
        }
    }

    if (stm == white && castling == 15 && enpassant == no_sq) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }

    if (hmclock == 0 && fmclock == 1) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }

    if (!fail) {
        std::cout << "[+] FEN PARSING TESTS PASSED\n"; 
    } else {
        std::cout << "[X] FEN PARSING TESTS FAILED\n"; 
    }

    return fail;
}
