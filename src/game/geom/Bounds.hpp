#pragma once

#include "game/types/Bounds.hpp"
#include "game/types/Point.hpp"

namespace game::geom
{
types::Bounds calcBounds(const PointsF& points);
} // namespace game::geom
