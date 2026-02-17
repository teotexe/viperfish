#include "attack_gen_test.hpp"

bool run_mask_gen_tests () {
    std::cout << "[?] MASK GENERATION TESTS RUNNING\n"; 
    bool fail = false;

    create_bishop_masks(bishop_masks);
    if (bishop_masks[a1] == 0x0040201008040200ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (bishop_masks[d4] == 0x0040221400142200ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }

    create_rook_masks(rook_masks);
    if (rook_masks[a1] == 0x000101010101017EULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (rook_masks[d4] == 0x0008080876080800ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }

    if (!fail) {
        std::cout << "[+] MASK GENERATION TESTS PASSED\n";
    } else {
        std::cout << "[X] MASK GENERATION TESTS FAILED\n";
    }

    return fail;
}

bool run_attack_gen_tests () {
    std::cout << "[?] ATTACK GENERATION TESTS RUNNING\n"; 
    bool fail = false;

    create_pawn_attacks(pawn_att);
    if (pawn_att[white][h2] == 0x0000000000400000ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (pawn_att[white][b7] == 0x0500000000000000ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (pawn_att[black][a2] == 0x0000000000000002ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (pawn_att[black][c5] == 0x000000000A000000ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }

    create_knight_attacks(knight_att);
    if (knight_att[d6] == 0x1422002214000000ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (knight_att[c7] == 0x1100110A00000000ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (knight_att[h8] == 0x0020400000000000ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }

    create_king_attacks(king_att);
    if (king_att[h1] == 0x000000000000C040ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (king_att[g4] == 0x000000E0A0E00000ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (king_att[a8] == 0x0203000000000000ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }

    create_bishop_attacks(bishop_att);
    create_rook_attacks(rook_att);

    if (get_bishop_att(c1, 0ULL) == 0x0000804020110A00ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (get_bishop_att(e4, 0x0000440000004400ULL) == 0x0000442800284400ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (get_rook_att(d4, 0ULL) == 0x08080808F7080808ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (get_rook_att(a1, 0x00000000000000FEULL) == 0x0101010101010102ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (get_queen_att(e5, 0x44004000400ULL) == 10544114677918765585ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }

    if (!fail) {
        std::cout << "[+] ATTACK GENERATION TESTS PASSED\n";
    } else {
        std::cout << "[X] ATTACK GENERATION TESTS FAILED\n";
    }

    return fail;
}
