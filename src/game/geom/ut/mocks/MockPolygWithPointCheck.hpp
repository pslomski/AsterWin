#pragma once

#include "common/testing/mocks.hpp"
#if not USE_ORIG(game_geom_PolygWithPointCheck)

#include <gmock/gmock.h>
#include "game/types/Types.hpp"

class MockPolygWithPointCheck
{
public:
    MOCK_METHOD(bool, checkPolygWithPoint, (const Object*, const Object*), (const));
};

inline bool checkPolygWithPoint(const Object* point, const Object* polygon)
{
    MockPolygWithPointCheck mock;
    return mock.checkPolygWithPoint(point, polygon);
}

#endif // game_geom_PolygWithPointCheck
