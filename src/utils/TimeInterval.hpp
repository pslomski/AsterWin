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
    void Reset() { elapsed = 0.0; }
    void Reset(Float NewInterval)
    {
        elapsed = 0.0;
        interval = NewInterval;
    }
    void SetExpired() { elapsed = interval + 0.01; }
    bool Inc(Float dt)
    {
        elapsed += dt;
        return elapsed >= interval;
    }
    Float Ratio() { return elapsed / interval; }

    Float interval;
    Float elapsed{};
};
} // namespace utils
