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

    return EXIT_SUCCESS;
}
