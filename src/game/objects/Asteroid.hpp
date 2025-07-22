#pragma once

#include <list>
#include "Bonuses.hpp"
#include "audio/SfxSample.hpp"
#include "game/objects/Asteroids.hpp"
#include "game/objects/Object.hpp"
#include "game/objects/Objects.hpp"
#include "game/objects/TempObjects.hpp"

namespace game
{
class Asteroid : public Object
{
private:
    int m_iSound;

public:
    Asteroid(int in_iLevel);
    virtual ~Asteroid();
    void Create();
    void Crash(Asteroids& vecAster, TempObjects& vecDebris, TvecBonus& vecBonus);
    static bool CreateBonus;
    int Level;
    bool HasBonus;
    SfxSample sndCrash;

protected:
    void onRender() override;
};
} // namespace game
