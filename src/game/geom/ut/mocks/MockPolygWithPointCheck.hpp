#pragma once

#include "common/testing/mocks.hpp"
#if not USE_ORIG(game_geom_PolygWithPointCheck)

#include <gmock/gmock.h>
#include "game/objects/Object.fwd.hpp"

namespace game::geom
{
class MockPolygWithPointCheck
{
public:
    MOCK_METHOD(bool, checkPolygWithPoint, (const objects::Object*, const objects::Object*), (const));
};

inline bool checkPolygWithPoint(const objects::Object* point, const objects::Object* polygon)
{
    return true;
}
} // namespace game::geom

#endif // game_geom_PolygWithPointCheck
