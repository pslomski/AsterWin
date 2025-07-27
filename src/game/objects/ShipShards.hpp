#pragma once

#include "game/objects/TempObject.hpp"
namespace game::objects
{
class ShipShards : public TempObject
{
public:
    ShipShards();

    void update() override;

private:
    void onRender() const override;

    float ratio{1.0f};
};
} // namespace game::objects
