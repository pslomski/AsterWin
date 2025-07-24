#include "Application.hpp"
#include <algorithm>
#include <math.h>
#include "audio/AudioLib.hpp"
#include "audio/Sound.hpp"
#include "game/World.hpp"
#include "ui/MainWindow.hpp"
#include "ui/Settings.hpp"

Application::Application(HINSTANCE hInstance) : hInstance(hInstance)
{
    AddFontResourceEx("Vectorb.ttf", FR_PRIVATE, 0);
    audio::audioLib.init();
    geSound.open();
    geMusic.open();
    Settings settings;
    settings.load();
    geWorld.soundVol = settings.soundVol;
    geWorld.musicVol = settings.musicVol;
    geMusic.setVolume(0.1f * geWorld.musicVol);
    geSound.setVolume(0.1f * geWorld.soundVol);
    geWorld.scrHeight = std::min(geWorld.scrHeight, int(0.80 * GetSystemMetrics(SM_CYSCREEN)));
    geWorld.scrWidth = geWorld.scrHeight;
}

Application::~Application()
{
    geSound.close();
    geMusic.close();
    audio::audioLib.free();
    RemoveFontResource("Vectorb.ttf");
}

void Application::run()
{
    MainWindow mainWindow(geWorld.scrWidth, geWorld.scrHeight);
    MSG message{};
    const double dt = 0.01;
    geWorld.dt = dt;
    double accumulator = 0.0;
    double currentTime = geWorld.getCurrentTime();
    while (message.message != WM_QUIT)
    {
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        double newTime = geWorld.getCurrentTime();
        double frameTime = newTime - currentTime;
        if (frameTime > 0.25) frameTime = 0.25;
        currentTime = newTime;
        accumulator += frameTime;
        while (accumulator >= dt)
        {
            mainWindow.update(dt);
            accumulator -= dt;
        }
        geWorld.interp = accumulator / dt;
        mainWindow.draw();
    }
}
