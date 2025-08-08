#pragma once

#include <cstdlib>
#include <ctime>

inline void randSeed()
{
    std::srand(std::time(0));
}

// random integer number 0..n-1
// TODO: replace with std::uniform_int_distribution
#define RAND(n) (rand() % (n))
