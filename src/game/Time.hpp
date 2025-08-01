#pragma once

#include "game/TimeDelta.hpp"
namespace game
{
class Time
{
public:
    Time();

    double getCurrentTime();

    TimeDelta dt{0.001}; // simulation step
    double accumulator{0.0};

private:
    double frequency{1.0};
};

inline Time time;
} // namespace game
