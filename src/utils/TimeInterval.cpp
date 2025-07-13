#include "TimeInterval.hpp"

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
} // namespace utils
