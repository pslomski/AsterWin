#pragma once

#include <random>

// random integer number 0..n-1
inline int randi(const int range)
{
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, range - 1);
    return dist(gen);
}
