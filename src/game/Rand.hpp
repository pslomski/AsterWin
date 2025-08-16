#pragma once

#include <random>

namespace game
{
// random integer number 0..n-1
inline int rand(const int range)

{
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, range - 1);
    return dist(gen);
}

// random float number min..max
inline float rand(const float min, const float max)
{
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}
} // namespace game
