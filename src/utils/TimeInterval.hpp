#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockTimeInterval.hpp"
#if USE_ORIG(utils_TimeInterval)

#include "game/types/Types.hpp"

namespace utils
{
class TimeInterval
{
public:
    TimeInterval();
    TimeInterval(const Float intervalArg);

    Float get() const { return interval; }
    void reset();
    void reset(const Float newInterval);
    bool inc(const Float dt);
    bool expired() const;
    Float ratio() const;

    Float interval;
    Float elapsed{};
};
} // namespace utils

#endif // utils_TimeInterval
