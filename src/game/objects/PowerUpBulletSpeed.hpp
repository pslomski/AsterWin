#pragma once

#include "game/objects/PowerUp.hpp"
#include "game/objects/Ship.fwd.hpp"

namespace game::objects
{
class PowerUpBulletSpeed : public PowerUp
{
public:
    PowerUpBulletSpeed(Float in_Dutation) : PowerUp(in_Dutation){};
    Ship* pShip;

protected:
    void OnStart();
    void OnStop();
};
} // namespace game::objects
