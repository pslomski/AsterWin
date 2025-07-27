#include "Tools.hpp"
#include <filesystem>
#include <windows.h>

std::string getAppDir()
{
    char filepath[MAX_PATH];
    GetModuleFileName(NULL, filepath, MAX_PATH);
    std::filesystem::path path(filepath);
    return path.parent_path().string();
}
