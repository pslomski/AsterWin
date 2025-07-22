#pragma once

#include "game/objects/BonusObject.hpp"
#include "game/types/BonusType.hpp"

namespace game::objects
{
using TvecBonus = std::list<BonusObject*>;

BonusObject* createBonusObj(BonusType in_Type);
} // namespace game::objects
