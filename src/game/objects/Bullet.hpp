#pragma once

#include "game/objects/TempObject.hpp"

namespace game::objects
{
class Bullet : public TempObject
{
public:
    Bullet();

private:
    void onRender() override;
};

typedef std::list<Bullet*> TvecBullet;
} // namespace game::objects
