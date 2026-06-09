#pragma once

#include "State.hpp"
#include "game/TimeDelta.hpp"

namespace ui
{
class StateManager
{
public:
    void changeState(State* newState);
    State* getActiveState() { return activeState; }
    void onKeyDown(SDL_Scancode key);
    void onKeyUp(SDL_Scancode key);
    void onChar(char ch);
    void onResize(int cx, int cy);
    void update(const game::TimeDelta dt);
    void draw();

private:
    State* activeState{nullptr};
};
} // namespace ui
