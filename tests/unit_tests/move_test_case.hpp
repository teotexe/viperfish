#pragma once

#include <string>
#include <vector>

#include "utils.hpp"

typedef struct {
    std::string fen;
    std::vector<Type::move> moves;
} MoveTestCase;
