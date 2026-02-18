#include "epd_parser.hpp"

void parse_epd (const std::string &filepath, std::vector<PerftCase> &cases) {
    std::ifstream file(filepath);
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        PerftCase perftcase;
        size_t start, split_pos;

        start = 0;
        split_pos = line.find(';');
        if (split_pos == std::string::npos) continue;

        perftcase.fen = line.substr(start, split_pos - start);

        start = line.find('D', split_pos);
        if (start == std::string::npos) continue;
        start++;

        split_pos = line.find(' ', start);
        if (split_pos == std::string::npos) continue;

        try {
            perftcase.depth = std::stoi(line.substr(start, split_pos - start));
            perftcase.expected = std::stoull(line.substr(split_pos + 1));
        } catch (const std::exception &e) {
            continue;
        }
        
        cases.emplace_back(perftcase);
    }
}
