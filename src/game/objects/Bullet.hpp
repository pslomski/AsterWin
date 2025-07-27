#pragma once

#include "game/objects/Bullet.fwd.hpp"
#include "game/objects/TempObject.hpp"

namespace game::objects
{
class Bullet : public TempObject
{
public:
    Bullet();

private:
    void onRender() const override;
};
} // namespace game::objects
