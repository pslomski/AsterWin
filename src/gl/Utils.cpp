#include "Utils.hpp"
#include <SDL3/SDL.h>

bool setVSync(const int interval)
{
    return SDL_GL_SetSwapInterval(interval);
}

void setGlView(const int width, const int height, const bool topDown)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (topDown)
    {
        glOrtho(0, width, height, 0, -1, 1);
    }
    else
    {
        glOrtho(0, width, 0, height, -1, 1);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
