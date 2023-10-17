#pragma once

#include <windows.h>

class Application
{
public:
    Application(HINSTANCE hInstance);
    ~Application();

    void run();

private:
    HINSTANCE hInstance;
};
