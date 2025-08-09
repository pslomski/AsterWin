#pragma once

#include "game/types/Position.hpp"

namespace game::objects
{
struct ObjectState
{
    types::Position pos{};
    float angleRad{};
};
} // namespace game::objects
