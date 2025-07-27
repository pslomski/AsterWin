#pragma once

#include "game/objects/TempObject.hpp"

namespace game::objects
{
class AsterShards : public TempObject
{
public:
    AsterShards();

    void update() override;

private:
    void onRender() const override;

    float ratio{1.0f};
};
} // namespace game::objects
