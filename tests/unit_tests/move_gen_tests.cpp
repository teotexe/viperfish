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

bool run_rook_move_gen_test () {
    std::vector<MoveTestCase> cases;
    parse_epd("./rook_test.epd", cases);

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
        generate_moves_rook(move_list, stm);

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

bool run_knight_move_gen_test () {
    std::vector<MoveTestCase> cases;
    parse_epd("./knight_test.epd", cases);

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
        generate_moves_knight(move_list, stm);

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

bool run_bishop_move_gen_test () {
    std::vector<MoveTestCase> cases;
    parse_epd("./bishop_test.epd", cases);

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
        generate_moves_bishop(move_list, stm);

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

bool run_king_move_gen_test () {
    std::vector<MoveTestCase> cases;
    parse_epd("./king_test.epd", cases);

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
        generate_moves_king(move_list, stm);

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

bool run_queen_move_gen_test () {
    std::vector<MoveTestCase> cases;
    parse_epd("./queen_test.epd", cases);

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
        generate_moves_queen(move_list, stm);

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

bool run_sq_att_detection_test () {
    std::string fen = "r1b1kb2/ppp1np2/n1q3p1/3pp2r/3P2Bp/1PQ1PNP1/PBPN1P1P/R4KR1 w q - 0 1";
    parse_fen(fen);

    std::vector<std::vector<square>> attacked_squares = {
        {d1, e4, a3, a5, e2, c8, g5, g2, a4},
        {g5, g8, g7, d8, b5, g4, b4, b8, g3},
    };
    std::vector<std::vector<square>> safe_squares = {
        {c7, a7, f8, f6, b6, d5, g6, b5, d8},
        {c2, b3, a3, d3, a5, h3, f3, e3, c1},
    };

    auto t0 = std::chrono::steady_clock::now();

    std::cout << "Test running: 0.0%";
    for (state side = white; side <= black; side++) {
        for (square sq : attacked_squares[side]) {
            if (!is_sq_attacked(side, sq)) {
                std::cerr << "\n[FAIL]\n";
                std::cerr << "Failed test case: " << fen << "\n";
                std::cerr << "Square: " << hr_squares[sq] << "\n";
                return false;
            }
        }
    }
    std::cout << "\rTest running: 25.0%";

    for (state side = white; side <= black; side++) {
        for (square sq : safe_squares[side]) {
            if (is_sq_attacked(side, sq)) {
                std::cerr << "\n[FAIL]\n";
                std::cerr << "Failed test case: " << fen << "\n";
                std::cerr << "Square: " << hr_squares[sq] << "\n";
                return false;
            }
        }
    }
    std::cout << "\rTest running: 50.0%";

    attacked_squares.clear();
    safe_squares.clear();

    fen = "r2k1b1r/p1pp4/b2qpppn/nN1PN3/1p2P2p/5Q1P/PPP1BPP1/R1B2K1R w - - 0 1";
    parse_fen(fen);
    attacked_squares = {
        {b1, a7, d2, h5, e1, c4, f7, h2, e6},
        {h7, f7, g7, c5, e8, b5, b3, b8, g3},
    };
    safe_squares = {
        {b6, a6, b7, e7, b8, c5, g7, h7, c8},
        {a2, f4, h3, e3, d4, a4, d1, h2, g1},
    };

    for (state side = white; side <= black; side++) {
        for (square sq : attacked_squares[side]) {
            if (!is_sq_attacked(side, sq)) {
                std::cerr << "\n[FAIL]\n";
                std::cerr << "Failed test case: " << fen << "\n";
                std::cerr << "Square: " << hr_squares[sq] << "\n";
                return false;
            }
        }
    }
    std::cout << "\rTest running: 75.0%";

    for (state side = white; side <= black; side++) {
        for (square sq : safe_squares[side]) {
            if (is_sq_attacked(side, sq)) {
                std::cerr << "\n[FAIL]\n";
                std::cerr << "Failed test case: " << fen << "\n";
                std::cerr << "Square: " << hr_squares[sq] << "\n";
                return false;
            }
        }
    }

    auto t1 = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

    std::cout << "\rTest running: 100.0%\n";
    std::cout << "2 cases cleared in " << ms << "ms.\n";

    return true;
}
