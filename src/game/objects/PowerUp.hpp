#pragma once

#include "game/objects/Ship.fwd.hpp"
#include "utils/TimeInterval.hpp"

namespace game::objects
{
class PowerUp
{
public:
    PowerUp(const float interval);

    void start();
    void stop();
    bool isActive() const { return active; }

    Ship* pShip{};
    utils::TimeInterval duration;

private:
    virtual void onStart() = 0;
    virtual void onStop() = 0;

    bool active{false};
};
} // namespace game::objects
