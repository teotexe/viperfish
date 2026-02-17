#include "perft.hpp"
#include <chrono>

void perft_driver(uint8_t depth, uint64_t &nodes) {
    Moves move_list;
    generate_moves(move_list);

    if (depth == 1) {
        nodes += move_list.index;
        return;
    }

    for (uint8_t i = 0; i < move_list.index; i++) {
        move mv = move_list.moves[i];

        // Copy board state
        BoardState board;
        copy_board_state(board);

        // Make move
        make_move(mv);

        // Continue recursively
        perft_driver(depth - 1, nodes);

        // Restore board state
        restore_board_state(board);
    }
}

uint64_t run_perft (uint8_t depth) {
    Moves move_list;
    generate_moves(move_list);

    if (depth == 1) {
        return move_list.index;
    }

    uint64_t leaves = 0ULL;

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

void run_smoke_test () {
    const std::string filepath = "./smoke_test.epd";
    std::vector<PerftCase> cases;
    parse_epd(filepath, cases);

    bool success = true;

    for (const PerftCase &perftcase : cases) {
        parse_fen(perftcase.fen);
        uint64_t result = run_perft(perftcase.depth);
        if (result != perftcase.expected) {
            std::cerr << "[FAIL]\n";
            std::cerr << "expected: " << perftcase.expected << "\n";
            std::cerr << "result: " << result << "\n";
            success = false;
        }
    }

    if (success) {
        std::cout << cases.size() << " cases cleared.\n";
        std::cout << "[+] Smoke test passed.\n";
    } else {
        std::cout << "[X] Smoke test failed.\n";
    }
}

void run_standard_test () {

}

void run_stress_test () {

}

void run_manual_test (int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: perft --manual <depth> [\"<FEN>\"]\n";
        std::cerr << "If no FEN is provided the standard starting position is used.\n";
        return;
    }

    int depth = std::stoi(argv[2]);

    std::string fen;
    if (argc == 4) {
        fen = argv[3];
    } else {
        // Standard starting position
        fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    }

    parse_fen(fen);
    uint64_t result = run_perft(static_cast<uint8_t>(depth));
    std::cout << result << "\n";
}

int main (int argc, char* argv[]) {
    std::string commands[] = {
        "--help",
        "--smoke",
        "--standard",
        "--stress",
        "--manual",
    };

    if (argc < 2) {
        std::cerr << "Only one command is accepted.\n";
        std::cerr << "See 'perft --help'.\n";
        return EXIT_FAILURE;
    }

    std::string command = argv[1];
    if (command == "--help") {
        std::cout << "List of commands:\n";
        for (const std::string &cmmd : commands) {
            std::cout << cmmd << "\n";
        }

        return EXIT_SUCCESS;
    }

    init_board_state();

    auto t0 = std::chrono::steady_clock::now();

    if (command == "--smoke") {
        run_smoke_test();
    } else if (command == "--standard") {
        run_standard_test();
    } else if (command == "--stress") {
        run_stress_test();
    } else if (command == "--manual") {
        run_manual_test(argc, argv);
    } else {
        std::cerr << "Unknown command.\n";
        std::cerr << "See 'perft --help'.\n";
        return EXIT_FAILURE;
    }

    auto t1 = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout << "[*] " << command << " completed in " << ms << " ms\n";

    return EXIT_SUCCESS;
}
