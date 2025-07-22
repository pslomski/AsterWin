#include "PowerUpBulletSpeed.hpp"
#include "game/GameConsts.hpp"
#include "game/objects/Ship.hpp"

namespace game::objects
{
PowerUpBulletSpeed::PowerUpBulletSpeed(const float interval) : PowerUp(interval) {}

void PowerUpBulletSpeed::onStart()
{
    pShip->BulletSpeed += GE_BULLET_SPEED_INC;
}

void PowerUpBulletSpeed::onStop()
{
    pShip->BulletSpeed -= GE_BULLET_SPEED_INC;
}
} // namespace game::objects
