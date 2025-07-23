#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockPolygWithPointCheck.hpp"
#if USE_ORIG(game_geom_PolygWithPointCheck)

#include "game/objects/Object.hpp"

namespace game::geom
{
bool checkPolygWithPoint(const objects::Object* point, const objects::Object* polygon);
} // namespace game::geom
#endif // USE_ORIG(game_geom_PolygWithPointCheck)
