#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "move_test_case.hpp"

void parse_epd (const std::string &filepath, std::vector<MoveTestCase> &cases);
