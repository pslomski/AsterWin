#pragma once

#include "common/testing/mocks.hpp"
#if not USE_ORIG(game_World)

#include "game/types/Bounds.hpp"
#include "game/types/Types.hpp"

class World
{
public:
    Bounds bounds;
    Float interp;
};

inline World geWorld;

#endif // game_World
