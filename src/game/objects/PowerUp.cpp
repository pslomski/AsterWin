#include "PowerUp.hpp"

namespace game::objects
{
PowerUp::PowerUp(const float interval) : duration(interval) {}

void PowerUp::start()
{
    stop();
    onStart();
    active = true;
}

void PowerUp::stop()
{
    if (active)
    {
        onStop();
    }
    duration.reset();
    active = false;
}
} // namespace game::objects
