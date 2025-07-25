#pragma once

#include <windows.h>

namespace ui
{
class StateManager;

class State
{
public:
    State(StateManager* manager);
    virtual ~State(){};

    virtual void onKeyDown(WPARAM) {}
    virtual void onKeyUp(WPARAM) {}
    virtual void onChar(WPARAM) {}
    virtual void onResize([[maybe_unused]] const int cx, [[maybe_unused]] const int cy) {}
    virtual void update([[maybe_unused]] const double timeStep) {}
    virtual void draw() {}
    virtual void enterState() {}
    virtual void leaveState() {}

protected:
    void changeState(State* newState);

    StateManager* stateManager{nullptr};
};
} // namespace ui
