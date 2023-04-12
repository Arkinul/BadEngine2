//
// Created by heinr on 12.04.2023.
//

#include <cstdint>

#ifndef BADENGINE2_TYPES_H
#define BADENGINE2_TYPES_H

#endif //BADENGINE2_TYPES_H

typedef uint64_t Bitboard;

enum Direction {
    NORTH = 8,
    SOUTH = -8,
    WEST = -1,
    EAST = 1,
    NORTHWEST = 7,
    NORTHEAST = 9,
    SOUTHWEST = -9,
    SOUTHEAST = -7
};

