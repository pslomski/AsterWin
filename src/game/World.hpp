#pragma once

#include <windows.h>
#include "types/Bounds.hpp"
#include "types/Point.hpp"
#include "types/Types.hpp"

class World
{
private:
    bool usePerfCounter;
    double frequency;
    DWORD timePrev;
    DWORD time;
    void SetTime(DWORD inTime)
    {
        timePrev = time;
        time = inTime;
    };

public:
    World();
    virtual ~World();

    int musicVol;
    int soundVol;
    bool isGameRunning;
    Float dt; // krok czasu pomiedzy  wywolaniami GetTimeDelta
    Float interp; // wspczynnik interpolacji ruchu
    int scrWidth; // szerokosc ekranu w pixelach
    int scrHeight; // wysokosc ekranu w pixelach
    Bounds bounds;

    Float getWidth() { return bounds.width(); }
    Float getHeight() { return bounds.height(); }
    PointF getCenter() { return PointF((bounds.x0 + bounds.x1) / 2.0, (bounds.y0 + bounds.y1) / 2.0); }
    PointF getRandomPosAtEdge();
    DWORD getTicks() { return time; }
    Float getCurrentTime();
};

extern World geWorld;
