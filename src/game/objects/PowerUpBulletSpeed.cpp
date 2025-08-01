#include "PowerUpBulletSpeed.hpp"
#include "game/GameConsts.hpp"
#include "game/objects/Ship.hpp"

namespace game::objects
{
PowerUpBulletSpeed::PowerUpBulletSpeed(const float interval) : PowerUp(interval) {}

void PowerUpBulletSpeed::onStart()
{
    pShip->bulletSpeed = GE_INITIAL_BULLET_SPEED + GE_BULLET_SPEED_INC;
}

void PowerUpBulletSpeed::onStop()
{
    pShip->bulletSpeed = GE_INITIAL_BULLET_SPEED;
}
} // namespace game::objects
