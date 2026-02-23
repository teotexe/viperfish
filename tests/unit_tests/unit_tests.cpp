#include "unit_tests.hpp"

int main (void) {
    init_board_state();

    if (run_pawn_move_gen_test()) {
        std::cout << "[+] Pawn move gen test passed.\n";
    } else {
        std::cout << "[X] Pawn move gen test failed.\n";
        return EXIT_FAILURE;
    }

    if (run_rook_move_gen_test()) {
        std::cout << "[+] Rook move gen test passed.\n";
    } else {
        std::cout << "[X] Rook move gen test failed.\n";
        return EXIT_FAILURE;
    }

    if (run_knight_move_gen_test()) {
        std::cout << "[+] Knight move gen test passed.\n";
    } else {
        std::cout << "[X] Knight move gen test failed.\n";
        return EXIT_FAILURE;
    }

    if (run_bishop_move_gen_test()) {
        std::cout << "[+] Bishop move gen test passed.\n";
    } else {
        std::cout << "[X] Bishop move gen test failed.\n";
        return EXIT_FAILURE;
    }

    if (run_king_move_gen_test()) {
        std::cout << "[+] King move gen test passed.\n";
    } else {
        std::cout << "[X] King move gen test failed.\n";
        return EXIT_FAILURE;
    }

    if (run_queen_move_gen_test()) {
        std::cout << "[+] Queen move gen test passed.\n";
    } else {
        std::cout << "[X] Queen move gen test failed.\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
