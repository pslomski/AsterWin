#include "PowerUpAddBullets.hpp"
#include "game/GameConsts.hpp"
#include "game/objects/Ship.hpp"

namespace game::objects
{
void PowerUpAddBullets::OnStart()
{
    pShip->MaxBullets += GE_BULLETS_INCREMENT;
}

void PowerUpAddBullets::OnStop()
{
    pShip->MaxBullets -= GE_BULLETS_INCREMENT;
}
} // namespace game::objects
