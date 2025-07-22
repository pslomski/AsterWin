#include "PowerUpBulletSpeed.hpp"
#include "game/GameConsts.hpp"
#include "game/objects/Ship.hpp"

namespace game::objects
{
void PowerUpBulletSpeed::OnStart()
{
    pShip->BulletSpeed += GE_BULLET_SPEED_INC;
}

void PowerUpBulletSpeed::OnStop()
{
    pShip->BulletSpeed -= GE_BULLET_SPEED_INC;
}
} // namespace game::objects
