#pragma once

namespace game
{
class Time
{
public:
    Time();

    double getCurrentTime();

    double dt{0.001}; // simulation step

private:
    double frequency{1.0};
};

inline Time time;
} // namespace game
