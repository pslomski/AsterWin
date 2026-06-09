#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockTime.hpp"
#if USE_ORIG(game_Time)

#include "game/TimeDelta.hpp"
namespace game
{
class Time
{
public:
    double getCurrentTime() const;

    TimeDelta dt{0.001}; // simulation step
    double accumulator{0.0};
};

inline Time time;
} // namespace game

#endif // game_Time
