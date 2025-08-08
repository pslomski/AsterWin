#pragma once

#include "game/Consts.hpp"

namespace game
{
inline float degToRad(const float degrees)
{
    return degrees / 180.0f * pi;
}

inline float radToDeg(const float radians)
{
    return radians / pi * 180.0f;
}

inline float normalizeAngleRad(const float angleRad)
{
    float normalized = angleRad;
    while (normalized < 0.0f)
        normalized += pi * 2.0f;
    while (normalized >= pi * 2.0f)
        normalized -= pi * 2.0f;
    return normalized;
}
} // namespace game
