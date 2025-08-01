#include "PowerUpAddBullets.hpp"
#include "game/GameConsts.hpp"
#include "game/objects/Ship.hpp"

namespace game::objects
{
PowerUpAddBullets::PowerUpAddBullets(const float interval) : PowerUp(interval) {}

void PowerUpAddBullets::onStart()
{
    pShip->maxBullets = GE_INITIAL_MAX_BULLETS + GE_BULLETS_INCREMENT;
}

void PowerUpAddBullets::onStop()
{
    pShip->maxBullets = GE_INITIAL_MAX_BULLETS;
}
} // namespace game::objects
