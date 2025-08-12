#include "Time.hpp"
#include <windows.h>

namespace game
{
Time::Time()
{
    LARGE_INTEGER f;
    QueryPerformanceFrequency(&f);
    frequency = f.QuadPart;
}

double Time::getCurrentTime() const
{
    // TODO: use std::chrono
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return t.QuadPart / frequency;
}
} // namespace game
