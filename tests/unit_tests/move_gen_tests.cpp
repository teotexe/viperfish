#include "move_gen_tests.hpp"

bool compare_moves (std::vector<move> expected, Moves move_list) {
    if (move_list.index != expected.size()) {
        return false;
    }

    std::sort(move_list.moves, move_list.moves + move_list.index);
    for (uint8_t i = 0; i < move_list.index; i++) {
        if (move_list.moves[i] != expected[i]) {
            return false;
        }
    }

    return true;
}

void move_gen_setup () {
    pawn_mv_mask = 0ULL;

    find_checkers(!stm);
    if (countbits(checkers) == 1) {
        square king_sq = getls1b(bitboards[(stm == white) ? K : k]);
        square checker_sq = getls1b(checkers);
        legal_mv_mask = blocker_tables[king_sq][checker_sq] | checkers;

        if (stm == white) {
            if (enpassant != no_sq && (1ULL << (enpassant - 8)) & checkers) pawn_mv_mask |= (1ULL << enpassant);
        } else {
            if (enpassant != no_sq && (1ULL << (enpassant + 8)) & checkers) pawn_mv_mask |= (1ULL << enpassant);
        }
    } else {
        legal_mv_mask = 0xFFFFFFFFFFFFFFFFULL;
    }

    pawn_mv_mask |= legal_mv_mask;

    find_pins(stm);
}

bool run_pawn_move_gen_test () {
    std::vector<MoveTestCase> cases;
    parse_epd("./pawn_test.epd", cases);

    uint16_t progress = 0;
    float percentage;

    auto t0 = std::chrono::steady_clock::now();

    for (const MoveTestCase &testcase : cases) {
        percentage = (progress * 100.0f) / cases.size();
        std::cout << "\rTest running: ";
        std::cout << std::fixed << std::setprecision(1) << percentage << "%";
        std::cout << std::flush;

        parse_fen(testcase.fen);

        move_gen_setup();

        Moves move_list = {};
        if (stm == white) {
            generate_moves_white_pawn(move_list);
        } else {
            generate_moves_black_pawn(move_list);
        }

        // std::cout << "\nsize: " << move_list.index << "\n";
        // for (uint8_t i = 0; i < move_list.index; i++) {
        //     std::cout << "mv: " << move_list.moves[i] << "\n";
        // }

        // for (auto &e : testcase.moves) {
        //     std::cout << "mv: " << e << "\n";
        // }

        if (!compare_moves(testcase.moves, move_list)) {
            std::cerr << "\n[FAIL]\n";
            std::cerr << "Failed test case: " << testcase.fen << "\n";
            return false;
        }

        progress++;
    }

    auto t1 = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

    std::cout << "\rTest running: 100.0%\n";
    std::cout << cases.size() << " cases cleared in " << ms << " ms.\n";

    return true;
}
