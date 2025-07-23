#pragma once

#include "Bonuses.hpp"
#include "audio/SfxSample.hpp"
#include "game/objects/Asteroids.hpp"
#include "game/objects/Object.hpp"
#include "game/objects/TempObjects.hpp"

namespace game::objects
{
class Asteroid : public Object
{
public:
    Asteroid(const int levelArg);

    void crash(Asteroids& asteroids, TempObjects& shards, Bonuses& bonuses, const bool canCreateBonus);

private:
    void create();
    void onRender() override;

    SfxSample sndCrash;
    int level;
};
} // namespace game::objects
