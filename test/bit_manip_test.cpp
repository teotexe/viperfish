#include "bit_manip_test.hpp"

bool run_bit_manip_tests () {
    std::cout << "[?] BIT MANIPULATION TESTS RUNNING\n"; 
    bool fail = false;

    bboard bb = 0ULL;
    setbit(bb, 0);
    if (getbit(bb, 0) == true) {
        std::cout << "[PASS]\n";
    } else {
        std::cerr << "[FAIL]\n";
        fail = true;
    }
    if (countbits(bb) == 1) {
        std::cout << "[PASS]\n";
    } else {
        std::cerr << "[FAIL]\n";
        fail = true;
    }

    setbit(bb, 63);
    if (getbit(bb, 63) == true) {
        std::cout << "[PASS]\n";
    } else {
        std::cerr << "[FAIL]\n";
        fail = true;
    }
    if (countbits(bb) == 2) {
        std::cout << "[PASS]\n";
    } else {
        std::cerr << "[FAIL]\n";
        fail = true;
    }

    popbit(bb, 0);
    if (getbit(bb, 0) == false) {
        std::cout << "[PASS]\n";
    } else {
        std::cerr << "[FAIL]\n";
        fail = true;
    }
    if (getbit(bb, 63) == true) {
        std::cout << "[PASS]\n";
    } else {
        std::cerr << "[FAIL]\n";
        fail = true;
    }
    if (countbits(bb) == 1) {
        std::cout << "[PASS]\n";
    } else {
        std::cerr << "[FAIL]\n";
        fail = true;
    }

    bb = 0ULL;
    setbit(bb, 10);
    setbit(bb, 20);
    setbit(bb, 30);
    if (getls1b(bb) == 10) {
        std::cout << "[PASS]\n";
    } else {
        std::cerr << "[FAIL]\n";
        fail = true;
    }

    popbit(bb, 10);
    if (getls1b(bb) == 20) {
        std::cout << "[PASS]\n";
    } else {
        std::cerr << "[FAIL]\n";
        fail = true;
    }

    bb = 0xFFFFFFFFFFFFFFFF;
    if (countbits(bb) == 64) {
        std::cout << "[PASS]\n";
    } else {
        std::cerr << "[FAIL]\n";
        fail = true;
    }
    if (getls1b(bb) == 0) {
        std::cout << "[PASS]\n";
    } else {
        std::cerr << "[FAIL]\n";
        fail = true;
    }

    if (!fail) {
        std::cout << "[+] BIT MANIPULATION TESTS PASSED\n";
    } else {
        std::cerr << "[X] BIT MANIPULATION TESTS FAILED\n";
    }

    return fail;
}
