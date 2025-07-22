#include "PowerUpAddBullets.hpp"
#include "game/GameConsts.hpp"
#include "game/objects/Ship.hpp"

namespace game::objects
{
PowerUpAddBullets::PowerUpAddBullets(const float interval) : PowerUp(interval) {}

void PowerUpAddBullets::onStart()
{
    pShip->MaxBullets += GE_BULLETS_INCREMENT;
}

void PowerUpAddBullets::onStop()
{
    pShip->MaxBullets -= GE_BULLETS_INCREMENT;
}
} // namespace game::objects
