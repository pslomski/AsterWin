#pragma once

#include "common/testing/mocks.hpp"
#if not USE_ORIG(game_objects_Object)

#include <gmock/gmock.h>

namespace game::objects
{
class Object
{
public:
    virtual ~Object() {}
};
} // namespace game::objects

#endif // game_objects_Object
