#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "log/Log.hpp"
#include "ui/Application.hpp"
#include "utils/Tools.hpp"

int main(int, char**)
{
    try
    {
        Application app;
        LOG_INFO("Asteroids game started");
        LOG_INFO("current directory: %s", getAppDir().c_str());
        app.run();
        LOG_INFO("Asteroids game finished");
    }
    catch (std::exception& e)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
    }
    return 0;
}
