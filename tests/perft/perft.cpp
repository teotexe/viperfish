#include "perft.hpp"

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

bool run_test (const std::string &filepath) {
    std::vector<PerftCase> cases;
    parse_epd(filepath, cases);

    uint16_t progress = 0;
    float percentage;

    auto t0 = std::chrono::steady_clock::now();

    for (const PerftCase &perftcase : cases) {
        percentage = (progress * 100.0f) / cases.size();
        std::cout << "\rTest running: ";
        std::cout << std::fixed << std::setprecision(1) << percentage << "%";
        std::cout << std::flush;

        parse_fen(perftcase.fen);
        uint64_t result = run_perft(perftcase.depth);
        if (result != perftcase.expected) {
            std::cerr << "\n[FAIL]\n";
            std::cerr << "expected: " << perftcase.expected << "\n";
            std::cerr << "result: " << result << "\n";
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

int main (int argc, char* argv[]) {
    std::string commands[] = {
        "--help",
        "--manual <filepath>",
        "--smoke",
        "--standard",
        "--stress",
    };

    if (argc < 2) {
        std::cerr << "At least one argument needed.\n";
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

    if (command == "--manual" && argc != 3) {
        std::cerr << "Wrong number of arguments.\n";
        std::cerr << "See 'perft --help'.\n";
        return EXIT_FAILURE;
    }

    init_board_state();

    if (command == "--smoke") {
        if (run_test("./smoke_test.epd")) {
            std::cout << "[+] Smoke test passed.\n";
        } else {
            std::cout << "[X] Smoke test failed.\n";
        }
    } else if (command == "--standard") {
        if (run_test("./standard_test.epd")) {
            std::cout << "[+] Standard test passed.\n";
        } else {
            std::cout << "[X] Standard test failed.\n";
        }
    } else if (command == "--stress") {
        if (run_test("./stress_test.epd")) {
            std::cout << "[+] Stress test passed.\n";
        } else {
            std::cout << "[X] Stress test failed.\n";
        }
    } else if (command == "--manual") {
        std::string filepath = argv[2];
        if (run_test(filepath)) {
            std::cout << "[+] Manual test passed.\n";
        } else {
            std::cout << "[X] Manual test failed.\n";
        }
    } else {
        std::cerr << "Unknown command.\n";
        std::cerr << "See 'perft --help'.\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
