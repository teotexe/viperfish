#include "perft2.hpp"

// // Starting position
// constexpr const char* position1 = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
// // Tricky position
// constexpr const char* position2 = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
// // Killer position
// constexpr const char* position3 = "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1";

// constexpr const char* position4 = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1";
// constexpr const char* position5 = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
// constexpr const char* position6 = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
// constexpr const char* position7 = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";

// PERFT that outputs the results for the user
void run_user_perft (uint8_t depth) {
    std::cout << "PERFORMANCE TEST\n";

    Moves move_list;
    uint64_t nodes = 0;

    generate_moves(move_list);

    uint64_t start_time = get_time_ms();

    for (uint8_t i = 0; i < move_list.index; i++) {
        move mv = move_list.moves[i];

        // Copy board state
        BoardState board;
        copy_board_state(board);

        make_move(mv);

        uint64_t old_nodes = nodes;
        perft_driver(depth - 1, nodes);

        std::cout << hr_squares[get_mv_src(mv)] << hr_squares[get_mv_trgt(mv)] << ": " << nodes - old_nodes << " nodes\n";

        restore_board_state(board);
    }

    std::cout << "Depth: " << (uint32_t)depth << "\n";
    std::cout << "Nodes: " << nodes << "\n";
    std::cout << "Time: " << get_time_ms() - start_time << "\n";
}

uint64_t run_perft (uint8_t depth) {
    Moves move_list;
    uint64_t leaves = 0ULL;

    generate_moves(move_list);
    for (uint8_t i = 0; i < move_list.index; i++) {
        move mv = move_list.moves[i];

        BoardState board;
        copy_board_state(board);

        make_move(mv);
        perft_driver(depth - 1, leaves);

        restore_board_state(board);
    }

    return leaves;
}

bool run_perfts () {
    std::cout << "[?] PERFORMANCE TESTS RUNNING\n";
    bool success = true;

    constexpr const char* positions[7] = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
        "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1",
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
    };

    constexpr uint8_t depths[7] = {
        6, 5, 6, 7, 6, 5, 5,
    };

    constexpr uint64_t expected_values[7] = {
        119060324ULL,
        193690690ULL,
        969091629ULL,
        178633661ULL,
        706045033ULL,
        89941194ULL,
        164075551ULL,
    };

    for (uint8_t i = 0; i < 7; i++) {
        parse_fen(positions[i]);
        uint64_t res = run_perft(depths[i]);
        if (res != expected_values[i]) {
            std::cout << "[FAIL]: PERFT on position " << (uint32_t)i << " failed.\n";
            std::cout << res << "\n";
            success = false;
        }
    }

    if (success) {
        std::cout << "[+] PERFORMANCE TESTS PASSED\n";
    } else {
        std::cout << "[X] PERFORMANCE TESTS FAILED\n";
    }

    return success;
}
