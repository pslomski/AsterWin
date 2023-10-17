#include <windows.h>
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
        app.run();
    }
    catch (std::exception& e)
    {
        MessageBox(NULL, e.what(), "Error", MB_OK | MB_ICONEXCLAMATION);
    }
    return 0;
}
