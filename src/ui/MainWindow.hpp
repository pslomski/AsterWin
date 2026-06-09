#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "game/TimeDelta.hpp"
#include "ui/gstate/StateManager.hpp"

class MainWindow
{
public:
    MainWindow(int width, int height);
    ~MainWindow();

    void update(const game::TimeDelta dt);
    void draw();
    void onKeyDown(SDL_Scancode key);
    void onKeyUp(SDL_Scancode key);
    void onChar(char ch);
    void onSize(int width, int height);

    bool isActive{true};

private:
    void initGL();

    SDL_Window* window{nullptr};
    SDL_GLContext glContext{nullptr};
    ui::StateManager* stateManager{nullptr};
};
