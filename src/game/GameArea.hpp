#pragma once

#include "types/Bounds.hpp"
#include "types/Point.hpp"
#include "types/Types.hpp"

namespace game
{
class GameArea
{
public:
    Float width() const;
    Float height() const;
    int widthi() const;
    int heighti() const;
    PointF center() const;
    PointF randomPosAtEdge() const;

    Bounds bounds{.x0 = 0.0f, .y0 = 0.0f, .x1 = 90.0f, .y1 = 90.0f};
};

inline constexpr GameArea gameArea;
} // namespace game
