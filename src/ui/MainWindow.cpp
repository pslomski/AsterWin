#include "MainWindow.hpp"
#include "gl/Utils.hpp"
#include "log/Log.hpp"
#include "ui/Font.hpp"
#include "ui/gstate/MenuState.hpp"
#include "utils/Exception.hpp"

MainWindow::MainWindow(int width, int height)
{
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    window = SDL_CreateWindow("Asteroids 2010", width, height,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) throw EGenericError("Cannot create the main window");

    glContext = SDL_GL_CreateContext(window);
    if (!glContext) throw EGenericError("Cannot create OpenGL context");

    SDL_StartTextInput(window);
    initGL();
    onSize(width, height);
    stateManager = new ui::StateManager;
    stateManager->changeState(ui::CMenuState::getInstance(stateManager));
}

MainWindow::~MainWindow()
{
    delete stateManager;
    if (glContext) SDL_GL_DestroyContext(glContext);
    if (window) SDL_DestroyWindow(window);
}

void MainWindow::initGL()
{
    const auto isVsync = setVSync(0);
    LOG_INFO("isVsync:%d", isVsync);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void MainWindow::onSize(int width, int height)
{
    glViewport(0, 0, width, height);
}

void MainWindow::onKeyDown(SDL_Scancode key)
{
    stateManager->onKeyDown(key);
}

void MainWindow::onKeyUp(SDL_Scancode key)
{
    stateManager->onKeyUp(key);
}

void MainWindow::onChar(char ch)
{
    stateManager->onChar(ch);
}

void MainWindow::update(const game::TimeDelta dt)
{
    stateManager->update(dt);
}

void MainWindow::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    stateManager->draw();
    SDL_GL_SwapWindow(window);
}
