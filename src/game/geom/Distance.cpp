#include "Distance.hpp"
#include <cmath>

namespace game::geom
{
float distance(const types::Position& pos1, const types::Position& pos2)
{
    const auto dx = pos1.x - pos2.x;
    const auto dy = pos1.y - pos2.y;
    return std::sqrtf(dx * dx + dy * dy);
}
} // namespace game::geom
