#include "Bonuses.hpp"
#include "game/objects/BonusAddBullets.hpp"
#include "game/objects/BonusBulletSpeed.hpp"
#include "game/objects/BonusPoints.hpp"

namespace game::objects
{
game::objects::BonusObject* createBonusObj(BonusType in_Type)
{
    switch (in_Type)
    {
        case BonusType::Bullets:
            return new BonusAddBullets;
        case BonusType::BulletSpeed:
            return new BonusBulletSpeed;
        case BonusType::Points:
            return new BonusPoints;
    }
    return new BonusPoints;
}
} // namespace game::objects
