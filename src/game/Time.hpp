#pragma once

#include <locale>
namespace game
{
class Time
{
public:
    double dt{0.001}; // simulation step
};

inline Time time;
} // namespace game
