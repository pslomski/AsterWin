#include "World.hpp"
#include "GameConsts.hpp"
#include "Sound.hpp"

World geWorld;

World::World()
{
    LARGE_INTEGER f;
    usePerfCounter = QueryPerformanceFrequency(&f) != 0;
    frequency = double(f.QuadPart);
    musicVol = 5;
    soundVol = 5;
    isGameRunning = false;
    bounds.x0 = 0;
    bounds.x1 = GE_WORLD_WIDTH;
    bounds.y0 = 0;
    bounds.y1 = GE_WORLD_HEIGHT;
    scrWidth = GE_SCREEN_WIDTH;
    scrHeight = GE_SCREEN_HEIGHT;
}

World::~World() {}

PointF World::getRandomPosAtEdge()
{
    PointF pt;
    int iSide = rand() % 4;
    if (0 == iSide)
    {
        pt.x = geWorld.bounds.x0;
        pt.y = rand() % int(geWorld.bounds.height());
    }
    else if (1 == iSide)
    {
        pt.x = geWorld.bounds.x1;
        pt.y = rand() % int(geWorld.bounds.height());
    }
    else if (2 == iSide)
    {
        pt.x = rand() % int(geWorld.bounds.width());
        pt.y = geWorld.bounds.y0;
    }
    else
    {
        pt.x = rand() % int(geWorld.bounds.width());
        pt.y = geWorld.bounds.y1;
    }
    return pt;
}

Float World::getCurrentTime()
{
    if (usePerfCounter)
    {
        LARGE_INTEGER t;
        QueryPerformanceCounter(&t);
        return t.QuadPart / frequency;
    }
    else
    {
        return GetTickCount() / 1000.0;
    }
}
