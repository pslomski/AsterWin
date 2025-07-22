#pragma once

#include "game/objects/BonusObject.hpp"
#include "game/types/BonusType.hpp"

namespace game::objects
{
class TGEBonusPoints : public BonusObject
{
public:
    TGEBonusPoints();

private:
    void Create();
};

typedef std::list<BonusObject*> TvecBonus;
typedef std::list<BonusObject*>::iterator TvecBonusIt;

BonusObject* geCreateBonusObj(BonusType in_Type);
} // namespace game::objects
