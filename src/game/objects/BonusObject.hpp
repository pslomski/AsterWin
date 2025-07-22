#pragma once

#include "game/objects/TempObject.hpp"
#include "game/types/BonusType.hpp"
#include "utils/TimeInterval.hpp"

namespace game::objects
{
class BonusObject : public TempObject
{
public:
    void update() override;

    BonusType bonusType;
    BonusObject(BonusType in_Type);

private:
    void onRender() override;

    utils::TimeInterval tiRot;
    Float m_ColRatio;
};
} // namespace game::objects
