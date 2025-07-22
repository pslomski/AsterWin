#include "TimeInterval.hpp"
#include <cassert>

namespace utils
{
TimeInterval::TimeInterval(const Float intervalArg) : interval(intervalArg) {}

TimeInterval::TimeInterval() : interval(1.0f) {}

void TimeInterval::reset()
{
    elapsed = 0.0f;
}

void TimeInterval::reset(const Float newInterval)
{
    elapsed = 0.0f;
    interval = newInterval;
}

bool TimeInterval::inc(const Float dt)
{
    elapsed += dt;
    return elapsed >= interval;
}

Float TimeInterval::ratio() const
{
    assert(interval > 0.0f && "Interval must be greater than zero to calculate ratio.");
    return elapsed / interval;
}
} // namespace utils
