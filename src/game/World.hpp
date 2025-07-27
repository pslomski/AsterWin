#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockWorld.hpp"
#if USE_ORIG(game_World)

#include "types/Types.hpp"

class World
{
public:
    World();

    Float interp; // TODO: Move to Object
    int scrWidth; // TODO: Move to a new class Screen
    int scrHeight; // TODO: Move to a new class Screen
};

extern World geWorld;

#endif // game_World
