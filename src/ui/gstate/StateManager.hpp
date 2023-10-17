#pragma once

#include "State.hpp"

namespace ui
{
class StateManager
{
public:
    void changeState(State* newState);
    State* getActiveState() { return activeState; }
    void onKeyDown(WPARAM wKey);
    void onKeyUp(WPARAM wKey);
    void onChar(WPARAM wChar);
    void onResize(int cx, int cy);
    void update(double timeStep);
    void draw();

private:
    State* activeState{nullptr};
};
} // namespace ui
