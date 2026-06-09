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

void StateManager::onKeyDown(SDL_Scancode key)
{
    if (activeState) activeState->onKeyDown(key);
}

void StateManager::onKeyUp(SDL_Scancode key)
{
    if (activeState) activeState->onKeyUp(key);
}

void StateManager::onChar(char ch)
{
    if (activeState) activeState->onChar(ch);
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
