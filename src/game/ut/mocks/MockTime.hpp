#pragma once

#include "common/testing/mocks.hpp"
#if not USE_ORIG(game_Time)

#include <gmock/gmock.h>
#include "game/TimeDelta.hpp"

namespace game
{
class Time
{
public:
    Time() {}

    MOCK_METHOD(double, getCurrentTime, (), (const));

    TimeDelta dt{0.001}; // simulation step
    double accumulator{0.0};
};

inline Time time;
} // namespace game

#endif // game_Time
