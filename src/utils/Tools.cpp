#include "Tools.hpp"
#include <SDL3/SDL.h>
#include <string>

std::string getAppDir()
{
    const char* basePath = SDL_GetBasePath();
    if (!basePath) return "";
    std::string result = basePath;
    while (!result.empty() && (result.back() == '/' || result.back() == '\\'))
        result.pop_back();
    return result;
}
