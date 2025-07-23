#pragma once

#include "Bullet.hpp"
#include "audio/SfxSample.hpp"
#include "game/objects/Bullets.hpp"
#include "game/objects/Object.hpp"
#include "game/objects/TempObjects.hpp"

namespace game::objects
{
class Ufo : public Object
{
public:
    Ufo();
    virtual ~Ufo();
    void update() override;
    Object* pShip; // wskaznik na statek gracza (ustawiane przy okazji poruszania
                   // obiektow)
    Object* pAster; // wskaznik na najblizsza asteroide (ustawiane przy okazji
                    // poruszania obiektow)
    void Action(Bullets& bullets);
    Bullet* FireBullet(const PointF& pt);
    void Crash(TempObjects& vecObiekty);
    SfxSample sndEngine;
    SfxSample sndCrash;

private:
    void onRender() override;

    Float CheckTime;
    Float CheckTimeElapsed;
    Float MoveTime;
    Float MoveTimeElapsed;
    Float FireTime;
    Float FireTimeElapsed;
};
} // namespace game::objects
