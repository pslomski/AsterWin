#include "Utils.hpp"
#include <windows.h>

bool setVSync(const int interval)
{
    auto extensions = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS));
    if (strstr(extensions, "WGL_EXT_swap_control") == 0)
    {
        return false;
    }
    else
    {
        using WglSwapIntervalEXT = bool(APIENTRY*)(int);
        auto wglSwapIntervalEXT =
            reinterpret_cast<WglSwapIntervalEXT>(reinterpret_cast<void*>(wglGetProcAddress("wglSwapIntervalEXT")));
        if (wglSwapIntervalEXT)
        {
            wglSwapIntervalEXT(interval);
        }
        using WglGetSwapIntervalEXT = int (*)();
        auto wglGetSwapIntervalEXT = reinterpret_cast<WglGetSwapIntervalEXT>(
            reinterpret_cast<void*>(wglGetProcAddress("wglGetSwapIntervalEXT")));
        if (wglGetSwapIntervalEXT)
        {
            return wglGetSwapIntervalEXT() > 0;
        }
        else
        {
            return false;
        }
    }
}
