#include "attack_gen_test.hpp"

bool run_mask_gen_tests () {
    std::cout << "[?] MASK GENERATION TESTS RUNNING\n"; 
    bool fail = false;

    bboard bishop_masks[64] = {0};
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

    bboard rook_masks[64] = {0};
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

    if (!fail) {
        std::cout << "[+] ATTACK GENERATION TESTS PASSED\n";
    } else {
        std::cout << "[X] ATTACK GENERATION TESTS FAILED\n";
    }

    return fail;
}
