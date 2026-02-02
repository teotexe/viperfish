#include "utils.hpp"

// Ranks assumed to be 0-7
int8_t getrank (square sq) {
    return sq / 8;
}

// Files assumed to be 0-7
int8_t getfile (square sq) {
    return sq % 8;
}
