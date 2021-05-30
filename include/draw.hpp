#ifndef _DRAW_INCLUDE_DRAW_HPP_
#define _DRAW_INCLUDE_DRAW_HPP_

extern "C" {

#include <cstdint>

struct Position {
    int32_t x;
    int32_t y;
};

struct Heading {
    int x;
    int y;
};

enum DirectionChange {
    LEFT = -1,
    STRAIGHT = 0,
    RIGHT = 1,
};

struct Agent {
    DirectionChange (*Next)(uint64_t step, Position pos, Heading heading);
};

void GetHandle(Agent* agent);

}  // extern "C"

#endif  // _DRAW_INCLUDE_DRAW_HPP_
