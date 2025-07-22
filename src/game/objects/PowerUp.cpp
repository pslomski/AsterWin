#include "PowerUp.hpp"

namespace game::objects
{
PowerUp::PowerUp(const float interval) : Duration(interval) {}

void PowerUp::Start()
{
    Stop();
    OnStart();
    active = true;
}

void PowerUp::Stop()
{
    if (active)
    {
        OnStop();
    }
    Duration.reset();
    active = false;
}
} // namespace game::objects
