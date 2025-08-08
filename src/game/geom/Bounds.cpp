#include "Bounds.hpp"
#include <cmath>

namespace game::geom
{
types::Bounds calcBounds(const PointsF& points)
{
    float max = 0.0f;
    for (const auto& point : points)
    {
        max = std::max(max, std::sqrtf(point.x * point.x + point.y * point.y));
    }
    max *= 1.05f; // add some margin
    return {.x0 = -max, .y0 = -max, .x1 = max, .y1 = max};
}
} // namespace game::geom
