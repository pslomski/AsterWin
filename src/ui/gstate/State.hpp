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
    virtual void onResize(int cx, int cy) {}
    virtual void update(double timeStep) {}
    virtual void draw() {}
    virtual void enterState() {}
    virtual void leaveState() {}

protected:
    void changeState(State* newState);

    StateManager* stateManager;
};
} // namespace ui
