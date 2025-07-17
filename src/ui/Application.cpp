#include "Application.hpp"
#include <algorithm>
#include <math.h>
#include "audio/Sound.hpp"
#include "game/World.hpp"
#include "ui/MainWindow.hpp"
#include "ui/Settings.hpp"

Application::Application(HINSTANCE hInstance) : hInstance(hInstance)
{
    AddFontResourceEx("Vectorb.ttf", FR_PRIVATE, 0);
    geSound.open();
    geMusic.open();
    Settings settings;
    settings.load();
    geWorld.soundVol = settings.soundVol;
    geWorld.musicVol = settings.musicVol;
    geMusic.setVolume(0.1f * geWorld.soundVol);
    geSound.setVolume(0.1f * geWorld.musicVol);
    geWorld.scrHeight = std::min(geWorld.scrHeight, int(0.80 * GetSystemMetrics(SM_CYSCREEN)));
    geWorld.scrWidth = geWorld.scrHeight;
}

Application::~Application()
{
    geSound.close();
    geMusic.close();
    RemoveFontResource("Vectorb.ttf");
}

void Application::run()
{
    int vRefresh = GetDeviceCaps(::GetDC(0), VREFRESH);
    if (vRefresh < 2) vRefresh = 60;
    Float frameTime = (1000.0 / vRefresh);

    MainWindow mainWindow(geWorld.scrWidth, geWorld.scrHeight);
    MSG message;
    message.message = ~WM_QUIT;
    DWORD sleep = int(frameTime) - 1;

    Float dt = 0.0; // czas od ostatniej aktualizacji
    Float lastUpdateTime = geWorld.getCurrentTime(); // czas ostatniej aktualizacji
    Float accumulator = 0.0;
    const Float timeStep = 1.0 / vRefresh; // krok czasowy, a zarazem czas trwania ramki fizyki w
                                           // sekundach; tutaj czas trwania jednego cyklu
                                           // odswierzania ekranu
    const Float maxAccumulatedTime = 1.0; // maksymalny czas zgromadzony w pojedynczym
                                          // obiegu petli glownej
    while (message.message != WM_QUIT)
    {
        if (!geWorld.isGameRunning) MsgWaitForMultipleObjectsEx(0, NULL, sleep, QS_ALLEVENTS, 0);
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        dt = geWorld.getCurrentTime() - lastUpdateTime;
        lastUpdateTime += dt;
        dt = std::max(static_cast<Float>(0), dt);
        accumulator += dt;
        accumulator = std::ranges::clamp(accumulator, static_cast<Float>(0), maxAccumulatedTime);
        while (accumulator > timeStep)
        {
            geWorld.dt = timeStep;
            mainWindow.update(timeStep);
            accumulator -= timeStep;
        }
        geWorld.interp = accumulator / timeStep;
        mainWindow.draw();
    }
}
