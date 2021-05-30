#include "draw.hpp"

#include <random>

extern "C" {

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<int> distrib(-1, 1);

DirectionChange Next(uint64_t step, Position pos, Heading heading) {
    return static_cast<DirectionChange>(distrib(rd));
}

void GetHandle(Agent* agent) {
    agent->Next = &Next;
}

}  // extern "C"
