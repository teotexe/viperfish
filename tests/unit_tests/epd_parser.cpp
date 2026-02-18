#include "epd_parser.hpp"

void parse_epd (const std::string &filepath, std::vector<MoveTestCase> &cases) {
    std::ifstream file(filepath);
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        MoveTestCase testcase;
        size_t start, split_pos;

        start = 0;
        split_pos = line.find(';');
        if (split_pos == std::string::npos) continue;

        testcase.fen = line.substr(start, split_pos - start);

        if (line.find('N', split_pos) != std::string::npos) {
            cases.emplace_back(testcase);
            continue;
        }

        start = line.find(' ', split_pos);
        if (start == std::string::npos) continue;
        start++;

        move mv;
        while ((split_pos = line.find(' ', start)) != std::string::npos) {
            mv = std::stoi(line.substr(start, split_pos - start));
            testcase.moves.emplace_back(mv);
            start = split_pos + 1;
        }
        mv = std::stoi(line.substr(start));
        testcase.moves.emplace_back(mv);

        cases.emplace_back(testcase);
    }
}
