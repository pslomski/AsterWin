#include <windows.h>
#include "log/Log.hpp"
#include "ui/Application.hpp"
#include "utils/Tools.hpp"

int APIENTRY WinMain(
    HINSTANCE hInstance,
    [[maybe_unused]] HINSTANCE hPrevInstance,
    [[maybe_unused]] LPTSTR lpCmdLine,
    [[maybe_unused]] int nCmdShow)
{
    try
    {
        SetCurrentDirectory(getAppDir().c_str());
        Application app(hInstance);
        LOG_INF("Asteroids game started");
        LOG_INF("current directory: %s", getAppDir().c_str());
        app.run();
        LOG_INF("Asteroids game finished");
    }
    catch (std::exception& e)
    {
        MessageBox(NULL, e.what(), "Error", MB_OK | MB_ICONEXCLAMATION);
    }
    return 0;
}
