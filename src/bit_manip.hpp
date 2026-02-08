#pragma once

#include <cassert>
#include "utils.hpp"

__always_inline void setbit (bboard &bb, square sq) {
    assert(0 <= sq && sq < 64);
    bb |= (1ULL << sq);
}

__always_inline void popbit (bboard &bb, square sq) {
    assert(0 <= sq && sq < 64);
    bb &= ~(1ULL << sq);
}

__always_inline bool getbit (bboard bb, square sq) {
    assert(0 <= sq && sq < 64);
    return bb & (1ULL << sq);
}

__always_inline uint8_t countbits (bboard bb) {
    #if defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
        return __builtin_popcountll(bb);
    #elif defined(_MSC_VER)
        return __popcnt64(bb);
    #else
        uint8_t count = 0;
        while (bb) {
            count++;
            bb &= bb - 1;
        }

        return count;
    #endif
}

__always_inline uint8_t getls1b (bboard bb) {
    assert(bb != 0);
    #if defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
        return __builtin_ctzll(bb);
    #else
        return countbits((bb & -bb) - 1);
    #endif
}
