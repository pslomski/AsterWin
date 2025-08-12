#pragma once

#include "game/types/Box.hpp"
#include "game/types/Vector.hpp"

namespace game::geom
{
BoxF transform(const float rotationAngleRad, const types::Vector& translation, const BoxF& seg);
} // namespace game::geom
