#pragma once

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "epd_parser.hpp"
#include "fen.hpp"
#include "move_gen.hpp"
#include "move_test_case.hpp"

bool run_pawn_move_gen_test ();
bool run_rook_move_gen_test ();
bool run_knight_move_gen_test ();
