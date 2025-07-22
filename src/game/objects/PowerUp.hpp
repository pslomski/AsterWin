#pragma once

#include "utils/TimeInterval.hpp"

namespace game::objects
{
class PowerUp
{
public:
    PowerUp(const float interval);

    void Start();
    void Stop();
    bool isActive() const { return active; }

    utils::TimeInterval Duration;

private:
    virtual void OnStart() = 0;
    virtual void OnStop() = 0;

    bool active{false};
};
} // namespace game::objects
