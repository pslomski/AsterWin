#pragma once

#include <GL/gl.h>
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

    bool isActive;

private:
    void registerWindowClass();
    void createContext();
    void initGL();
    void onSize(GLsizei width, GLsizei height);
    static LRESULT CALLBACK onEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam);
    void processEvent(UINT Message, WPARAM wParam, LPARAM lParam);

    HWND hWindow;
    HDC hDeviceContext;
    HGLRC hGLContext;
    ui::StateManager* stateManager;
};
