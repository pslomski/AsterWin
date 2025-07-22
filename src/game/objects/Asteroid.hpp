#pragma once

#include "Bonuses.hpp"
#include "audio/SfxSample.hpp"
#include "game/objects/Asteroids.hpp"
#include "game/objects/Object.hpp"
#include "game/objects/TempObjects.hpp"

namespace game
{
class Asteroid : public Object
{
public:
    Asteroid(const int levelArg);

    void crash(Asteroids& vecAster, TempObjects& vecDebris, TvecBonus& vecBonus, const bool canCreateBonus);

private:
    void create();
    void onRender() override;

    SfxSample sndCrash;
    int level;
};
} // namespace game
