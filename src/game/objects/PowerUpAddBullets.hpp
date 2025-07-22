#pragma once

#include "game/objects/PowerUp.hpp"

namespace game::objects
{
class PowerUpAddBullets : public PowerUp
{
public:
    PowerUpAddBullets(const float interval);

private:
    void onStart();
    void onStop();
};
} // namespace game::objects
