#pragma once

#include "game/objects/Object.hpp"
#include "game/types/Position.hpp"

namespace game::geom
{
float distance(const types::Position& pos1, const types::Position& pos2);
} // namespace game::geom
