#include "Application.hpp"
#include <SDL3/SDL.h>
#include "audio/AudioLib.hpp"
#include "audio/Sound.hpp"
#include "game/Time.hpp"
#include "game/TimeDelta.hpp"
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
    ui::viewport.height = 0.8f * GetSystemMetrics(SM_CYSCREEN);
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
    SDL_Event event{};
    const game::TimeDelta dt = 0.001; // 1 ms
    game::time.dt = dt;
    game::time.accumulator = 0.0;
    double currentTime = game::time.getCurrentTime();
    bool running = true;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    mainWindow.onKeyDown(event.key.scancode);
                    break;
                case SDL_EVENT_KEY_UP:
                    mainWindow.onKeyUp(event.key.scancode);
                    break;
                case SDL_EVENT_TEXT_INPUT:
                    if (event.text.text[0]) mainWindow.onChar(event.text.text[0]);
                    break;
                case SDL_EVENT_WINDOW_RESIZED:
                    mainWindow.onSize(event.window.data1, event.window.data2);
                    break;
            }
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
