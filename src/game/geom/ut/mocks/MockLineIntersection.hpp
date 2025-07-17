#pragma once

#include "common/testing/mocks.hpp"
#if not USE_ORIG(game_geom_LineIntersection)

#include <gmock/gmock.h>
#include "game/types/Box.hpp"
#include "game/types/Types.hpp"

class MockLineIntersection
{
public:
    MOCK_METHOD(int, linesIntersection, (const BoxF&, const BoxF&, Float&, Float&), (const));
};

inline int linesIntersection(const BoxF& o1, const BoxF& o2, Float& x, Float& y)
{
    MockLineIntersection mock;
    return mock.linesIntersection(o1, o2, x, y);
}

#endif // game_geom_LineIntersection
