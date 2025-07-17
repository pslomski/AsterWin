#pragma once

#include "game/types/Box.hpp"
#include "game/types/Types.hpp"

int linesIntersection(const BoxF& o1, const BoxF& o2, Float& x, Float& y);

#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockLineIntersection.hpp"
#if USE_ORIG(game_geom_LineIntersection)

#include "game/types/Box.hpp"
#include "game/types/Types.hpp"

int linesIntersection(const BoxF& o1, const BoxF& o2, Float& x, Float& y);

#endif // USE_ORIG(game_geom_LineIntersection)
