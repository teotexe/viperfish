#pragma once

#include <cstdint>

using bboard = uint64_t;
using state = uint8_t;
using square = uint8_t;
using counter = uint8_t;
using flag = uint8_t;
using piece = uint8_t;
using move = uint32_t;

enum { white, black, both };
enum { wk = 1, wq = 2, bk = 4, bq = 8 };
enum {
    P, R, N, B, K, Q,
    p, r, n, b, k, q,
};
enum {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8, no_sq,
};

// Not A file mask bitboard
constexpr bboard not_a_file = 18374403900871474942ULL;
// Not H file mask bitboard
constexpr bboard not_h_file = 9187201950435737471ULL;
// Not A, B files mask bitboard
constexpr bboard not_ab_files = 18229723555195321596ULL;
// Not H, G files mask bitboard
constexpr bboard not_gh_files = 4557430888798830399ULL;

int8_t getrank (square sq);
int8_t getfile (square sq);
