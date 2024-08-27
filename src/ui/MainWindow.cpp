#include "MainWindow.hpp"
#include "game/Consts.hpp"
#include "game/World.hpp"
#include "ui/Font.hpp"
#include "ui/gstate/MenuState.hpp"
#include "utils/Exception.hpp"
#include "utils/Tools.hpp"

#define WINDOW_CLASSNAME "ASTEROIDS"

MainWindow::MainWindow(int iWidth, int iHeight)
    : hWindow(NULL), hDeviceContext(NULL), hGLContext(NULL), stateManager(nullptr), isActive(true)
{
    registerWindowClass();

    RECT WindowRect;
    WindowRect.top = WindowRect.left = 0;
    WindowRect.right = iWidth;
    WindowRect.bottom = iHeight;

    DWORD dwStyle = WS_OVERLAPPEDWINDOW;
    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

    // Adjust the window to the true requested size
    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

    int x = GetSystemMetrics(SM_CXSCREEN) / 2 - (WindowRect.right - WindowRect.left) / 2;
    int y = GetSystemMetrics(SM_CYSCREEN) / 2 - (WindowRect.bottom - WindowRect.top) / 2;
    // Now create the main window
    hWindow = CreateWindowEx(
        dwExStyle,
        TEXT(WINDOW_CLASSNAME),
        TEXT("Asteroids 2010"),
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
        x,
        y,
        WindowRect.right - WindowRect.left,
        WindowRect.bottom - WindowRect.top,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        this);
    if (hWindow == NULL) throw EGenericError("Cannot create the main window");

    createContext();
    initGL();
    ShowWindow(hWindow, SW_SHOW);
    onSize(iWidth, iHeight);
    Font::SetDeviceContext(hDeviceContext);
    stateManager = new ui::StateManager;
    stateManager->changeState(ui::CMenuState::getInstance(stateManager));
}

MainWindow::~MainWindow()
{
    if (hGLContext)
    {
        // Make the rendering context not current
        wglMakeCurrent(NULL, NULL);
        // Delete the OpenGL rendering context
        wglDeleteContext(hGLContext);
        hGLContext = NULL;
    }
    if (hDeviceContext)
    {
        // Release the device context
        ReleaseDC(hWindow, hDeviceContext);
        hDeviceContext = NULL;
    }

    // Finally, destroy our main window and unregister the window class.
    DestroyWindow(hWindow);
    UnregisterClass(TEXT(WINDOW_CLASSNAME), GetModuleHandle(NULL));
}

LRESULT MainWindow::onEvent(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam)
{
    if (Message == WM_NCCREATE)
    {
        CREATESTRUCT* pCreateStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
        // Set as the "user data" parameter of the window
        SetWindowLongPtr(Handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
    }

    // Get the CMainWindow instance corresponding to the window handle
    MainWindow* pWindow = reinterpret_cast<MainWindow*>(GetWindowLongPtr(Handle, GWLP_USERDATA));
    if (pWindow) pWindow->processEvent(Message, wParam, lParam);

    switch (Message)
    {
        case WM_SYSCOMMAND:
            switch (wParam)
            {
                case SC_SCREENSAVE: // Screensaver Trying To Start?
                case SC_MONITORPOWER:; // Monitor Trying To Enter Powersave?
                    return 0; // Prevent From Happening
            }
            break;
        case WM_ACTIVATE:
        {
            if (!HIWORD(wParam)) // Check Minimization State
                pWindow->isActive = true;
            else
                pWindow->isActive = false;
            return 0;
        }
    }
    return DefWindowProc(Handle, Message, wParam, lParam);
}

void MainWindow::processEvent(UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch (Message)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            onSize(LOWORD(lParam), HIWORD(lParam));
            break;
        case WM_KEYDOWN:
            stateManager->onKeyDown(wParam);
            break;
        case WM_KEYUP:
            stateManager->onKeyUp(wParam);
            break;
        case WM_CHAR:
            stateManager->onChar(wParam);
            break;
    }
}

void MainWindow::registerWindowClass()
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASS WindowClass;
    WindowClass.style = 0;
    WindowClass.lpfnWndProc = &MainWindow::onEvent;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 0;
    WindowClass.hInstance = GetModuleHandle(NULL);
    WindowClass.hIcon = 0; // LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
    WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WindowClass.hbrBackground = 0;
    WindowClass.lpszMenuName = nullptr;
    WindowClass.lpszClassName = WINDOW_CLASSNAME;

    RegisterClass(&WindowClass);
}

void MainWindow::createContext()
{
    // Describes the pixel format of the drawing surface
    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1; // Version Number
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | // Draws to a window
        PFD_SUPPORT_OPENGL | // The format must support OpenGL
        PFD_DOUBLEBUFFER; // Support for double buffering
    pfd.iPixelType = PFD_TYPE_RGBA; // Uses an RGBA pixel format
    pfd.cColorBits = 32; // 32 bits colors

    hDeviceContext = ::GetDC(hWindow);
    if (!hDeviceContext) throw EGenericError("Unable to create rendering context");

    int PixelFormat;
    // Do Windows find a matching pixel format ?
    PixelFormat = ChoosePixelFormat(hDeviceContext, &pfd);
    if (!PixelFormat) throw EGenericError("Unable to create rendering context");
    // Set the new pixel format
    if (!SetPixelFormat(hDeviceContext, PixelFormat, &pfd)) throw EGenericError("Unable to create rendering context");
    // Create the OpenGL rendering context
    hGLContext = wglCreateContext(hDeviceContext);
    if (!hGLContext) throw EGenericError("Unable to create rendering context");
    // Activate the rendering context
    if (!wglMakeCurrent(hDeviceContext, hGLContext)) throw EGenericError("Unable to create rendering context");
}

void MainWindow::initGL()
{
    setVSync(1);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void MainWindow::onSize(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height); // Sets the size of the OpenGL viewport
}

void MainWindow::update(double dwCurrentTime)
{
    stateManager->update(dwCurrentTime);
}

void MainWindow::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    stateManager->draw();
    SwapBuffers(hDeviceContext);
}
