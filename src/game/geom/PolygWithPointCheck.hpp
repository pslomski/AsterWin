#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockPolygWithPointCheck.hpp"
#if USE_ORIG(game_geom_PolygWithPointCheck)

#include "game/objects/Object.hpp"

bool checkPolygWithPoint(const Object* point, const Object* polygon);

#endif // USE_ORIG(game_geom_PolygWithPointCheck)
