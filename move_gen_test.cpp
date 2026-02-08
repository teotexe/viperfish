#include "move_gen_test.hpp"

bool run_move_gen_tests () {
    std::cout << "[?] MOVE GENERATION TESTS RUNNING\n"; 
    bool fail = false;

    create_blocker_tables(blocker_tables);
    if (blocker_tables[b3][b4] == 0ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (blocker_tables[h1][a8] == 0x0002040810204000ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (blocker_tables[a1][a8] == 0x0001010101010100ULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }
    if (blocker_tables[a1][h1] == 0x000000000000007EULL) {
        std::cout << "[PASS]\n";
    } else {
        std::cout << "[FAIL]\n";
        fail = true;
    }

    if (!fail) {
        std::cout << "[+] MOVE GENERATION TESTS PASSED\n";
    } else {
        std::cout << "[X] MOVE GENERATION TESTS FAILED\n";
    }

    return fail;
}
