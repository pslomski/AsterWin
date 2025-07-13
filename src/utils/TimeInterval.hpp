#pragma once

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
    void setExpired();
    bool inc(const Float dt);
    Float ratio() const;

    Float interval;
    Float elapsed{};
};
} // namespace utils
