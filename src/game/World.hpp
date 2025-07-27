#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockWorld.hpp"
#if USE_ORIG(game_World)

class World
{
public:
    World();
};

extern World geWorld;

#endif // game_World
