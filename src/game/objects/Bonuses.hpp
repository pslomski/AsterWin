#pragma once

#include <list>
#include "game/objects/BonusObject.hpp"
#include "game/types/BonusType.hpp"

namespace game::objects
{
using Bonuses = std::list<BonusObject*>;

BonusObject* createBonusObj(const BonusType bonusType);
} // namespace game::objects
