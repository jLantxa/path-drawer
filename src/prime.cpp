#include <cmath>

#include "draw.hpp"

extern "C" {

bool is_prime(unsigned int number) {
    if (number < 2) {
        return false;
    }

    unsigned int sq = (unsigned int) sqrt(number);
    for (unsigned int n = 2; n <= sq; n++) {
        if ((number % n) == 0) {
            return false;
        }
    }

    return true;
}

DirectionChange Next(uint64_t step, Position pos, Heading heading) {
    if (is_prime(step)) {
        return DirectionChange::LEFT;
    }

    return DirectionChange::STRAIGHT;
}

void GetHandle(Agent* agent) {
    agent->Next = &Next;
}

}  // extern "C"
