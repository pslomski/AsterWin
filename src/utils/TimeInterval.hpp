#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockTimeInterval.hpp"
#if USE_ORIG(utils_TimeInterval)

namespace utils
{
class TimeInterval
{
public:
    TimeInterval();
    TimeInterval(const float intervalArg);

    float get() const { return interval; }
    void reset();
    void reset(const float newInterval);
    bool inc(const float dt);
    bool expired() const;
    float ratio() const;

    float interval;
    float elapsed{};
};
} // namespace utils

#endif // utils_TimeInterval
