#include "Application.hpp"
#include <windows.h>
#include "audio/AudioLib.hpp"
#include "audio/Sound.hpp"
#include "game/Time.hpp"
#include "game/objects/Object.hpp"
#include "ui/MainWindow.hpp"
#include "ui/Settings.hpp"
#include "ui/Viewport.hpp"

Application::Application()
{
    AddFontResourceEx("Vectorb.ttf", FR_PRIVATE, 0);
    audio::audioLib.init();
    geSound.open();
    geMusic.open();
    Settings settings;
    settings.load();
    geMusic.setVolume(0.1f * settings.musicVol);
    geSound.setVolume(0.1f * settings.soundVol);
    ui::viewport.height = 0.8 * GetSystemMetrics(SM_CYSCREEN);
    ui::viewport.width = ui::viewport.height;
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
    MainWindow mainWindow(ui::viewport.width, ui::viewport.height);
    MSG message{};
    const double dt = 0.001; // 1 ms
    game::time.dt = dt;
    game::time.accumulator = 0.0;
    double currentTime = game::time.getCurrentTime();
    while (message.message != WM_QUIT)
    {
        while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        double newTime = game::time.getCurrentTime();
        double frameTime = newTime - currentTime;
        if (frameTime > 0.25) frameTime = 0.25;
        currentTime = newTime;
        game::time.accumulator += frameTime;
        while (game::time.accumulator >= dt)
        {
            mainWindow.update(dt);
            game::time.accumulator -= dt;
        }
        game::objects::Object::interp = game::time.accumulator / dt;
        mainWindow.draw();
    }
}
