#pragma once

#include "common/testing/mocks.hpp"
#if not USE_ORIG(game_objects_Object)

#include <gmock/gmock.h>
#include "game/types/Types.hpp"

namespace game::objects
{
class Object
{
public:
    Object() {}
    virtual ~Object() {}

    static Float dt;
};

inline Float Object::dt = 0.0f;
} // namespace game::objects

#endif // game_objects_Object
