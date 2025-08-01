#include "StateManager.hpp"
#include "State.hpp"

namespace ui
{
void StateManager::changeState(State* newState)
{
    if (activeState) activeState->leaveState();
    activeState = newState;
    activeState->enterState();
}

void StateManager::onKeyDown(WPARAM wKey)
{
    if (activeState) activeState->onKeyDown(wKey);
}

void StateManager::onKeyUp(WPARAM wKey)
{
    if (activeState) activeState->onKeyUp(wKey);
}

void StateManager::onChar(WPARAM wChar)
{
    if (activeState) activeState->onChar(wChar);
}

void StateManager::onResize(int cx, int cy)
{
    if (activeState) activeState->onResize(cx, cy);
}

void StateManager::update(const game::TimeDelta dt)
{
    if (activeState) activeState->update(dt);
}

void StateManager::draw()
{
    if (activeState) activeState->draw();
}
} // namespace ui
