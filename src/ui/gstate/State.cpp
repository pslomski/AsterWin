#include "State.hpp"
#include "StateManager.hpp"

namespace ui
{
State::State(StateManager* manager) : stateManager(manager) {}

void State::changeState(State* newState)
{
    stateManager->changeState(newState);
}
} // namespace ui
