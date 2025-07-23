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
    void onRender() override;

    float ratio{1.0f};
    float size{1.0f};
};
} // namespace game::objects
