#pragma once

#include <string>
#include <vector>

#include "utils.hpp"

typedef struct {
    std::string fen;
    std::vector<move> moves;
} MoveTestCase;
