#include "bit_manip_test.hpp"
#include "attack_gen_test.hpp"
#include "move_gen_test.hpp"
#include "fen_test.hpp"
#include "perft2.hpp"

#include "debug.hpp"

int main (void) {
    if (run_bit_manip_tests()) exit(EXIT_FAILURE);
    if (run_mask_gen_tests()) exit(EXIT_FAILURE);
    if (run_attack_gen_tests()) exit(EXIT_FAILURE);
    if (run_move_gen_tests()) exit(EXIT_FAILURE);
    if (run_fen_tests()) exit(EXIT_FAILURE);
    
    // parse_fen(tricky_pos);
    // make_move(encode_move(e2, e3, P, 0, 0, 0, 0, 0));
    // make_move(encode_move(e7, e5, p, 0, 0, 1, 0, 0));
    // make_move(encode_move(d1, e2, Q, 0, 0, 0, 0, 0));
    // make_move(encode_move(e5, e4, p, 0, 0, 0, 0, 0));
    // make_move(encode_move(d2, d4, P, 0, 0, 1, 0, 0));
    // print_board();
    // std::cout << hr_squares[enpassant] << "\n";
    // run_user_perft(6);

    // Returns success
    if (!run_perfts()) exit(EXIT_FAILURE);
}
