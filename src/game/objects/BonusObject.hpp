#pragma once

#include "game/objects/TempObject.hpp"
#include "game/types/BonusType.hpp"
#include "utils/TimeInterval.hpp"

namespace game::objects
{
class BonusObject : public TempObject
{
public:
    BonusObject(const BonusType bonusTypeArg);

    void update() override;
    BonusType getBonusType() const { return bonusType; }

private:
    void onRender() const override;

    BonusType bonusType;
    utils::TimeInterval tiRot{1.5f};
    float colorRatio{1.0f};
};
} // namespace game::objects
