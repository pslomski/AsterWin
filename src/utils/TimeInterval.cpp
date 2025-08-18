#include "TimeInterval.hpp"
#include <cassert>

namespace utils
{
TimeInterval::TimeInterval(const float intervalArg) : interval(intervalArg) {}

TimeInterval::TimeInterval() : interval(1.0f) {}

void TimeInterval::reset()
{
    elapsed = 0.0f;
}

void TimeInterval::reset(const float newInterval)
{
    elapsed = 0.0f;
    interval = newInterval;
}

bool TimeInterval::inc(const float dt)
{
    elapsed += dt;
    return expired();
}

bool TimeInterval::expired() const
{
    return elapsed >= interval;
}

float TimeInterval::ratio() const
{
    assert(interval > 0.0f && "Interval must be greater than zero to calculate ratio.");
    return elapsed / interval;
}
} // namespace utils
