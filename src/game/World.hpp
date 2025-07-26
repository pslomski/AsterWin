#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockWorld.hpp"
#if USE_ORIG(game_World)

#include "types/Bounds.hpp"
#include "types/Point.hpp"
#include "types/Types.hpp"

class World
{
public:
    World();

    Float interp; // TODO: Move to Object
    int scrWidth; // TODO: Move to a new class Screen
    int scrHeight; // TODO: Move to a new class Screen
    Bounds bounds;

    Float getWidth() { return bounds.width(); }
    Float getHeight() { return bounds.height(); }
    PointF getCenter() { return PointF((bounds.x0 + bounds.x1) / 2.0, (bounds.y0 + bounds.y1) / 2.0); }
    PointF getRandomPosAtEdge();
};

extern World geWorld;

#endif // game_World
