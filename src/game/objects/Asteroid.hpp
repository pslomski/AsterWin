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

    int level;
    bool HasBonus;
    SfxSample sndCrash;

protected:
    void onRender() override;

private:
    void create();
};
} // namespace game
