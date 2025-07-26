#pragma once

#include <cstdlib>
#include <ctime>

inline void randSeed()
{
    std::srand(std::time(0));
}

// random integer number 0..n-1
#define RAND(n) (rand() % (n))
