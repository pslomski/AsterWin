#pragma once

#include "game/objects/PowerUp.hpp"

namespace game::objects
{
class PowerUpBulletSpeed : public PowerUp
{
public:
    PowerUpBulletSpeed(const float interval);

private:
    void onStart();
    void onStop();
};
} // namespace game::objects
