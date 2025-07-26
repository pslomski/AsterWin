#pragma once

#include <gl/gl.h>
#include <windows.h>
#include "ui/gstate/StateManager.hpp"

class MainWindow
{
public:
    MainWindow(int iWidth, int iHeight);
    ~MainWindow();

    void update(double dwCurrentTime);
    void draw();
    HDC getDC() { return hDeviceContext; }

    bool isActive{true};

private:
    void registerWindowClass();
    void createContext();
    void initGL();
    void onSize(GLsizei width, GLsizei height);
    static LRESULT CALLBACK onEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);
    void processEvent(UINT Message, WPARAM wParam, LPARAM lParam);

    HWND hWindow{NULL};
    HDC hDeviceContext{NULL};
    HGLRC hGLContext{NULL};
    ui::StateManager* stateManager{nullptr};
};
