#pragma once

#include "game/types/Types.hpp"

namespace utils
{
class TimeInterval
{
public:
    TimeInterval(Float in_Interval);
    TimeInterval()
    {
        Elapsed = 0.0;
        Interval = 1.0;
    }
    Float Interval;
    Float Elapsed;
    void Reset() { Elapsed = 0.0; }
    void Reset(Float NewInterval)
    {
        Elapsed = 0.0;
        Interval = NewInterval;
    }
    void SetExpired() { Elapsed = Interval + 0.01; }
    bool Inc(Float dt)
    {
        Elapsed += dt;
        return Elapsed >= Interval;
    }
    Float Ratio() { return Elapsed / Interval; }
};
} // namespace utils
