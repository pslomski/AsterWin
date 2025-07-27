#pragma once

#include "game/objects/TempObject.hpp"

namespace game::objects
{
class StarBlink : public TempObject
{
public:
    StarBlink();

    void update() override;

private:
    void init();
    void onRender() const override;

    float ratio{1.0f};
    float size{1.0f};
};
} // namespace game::objects
