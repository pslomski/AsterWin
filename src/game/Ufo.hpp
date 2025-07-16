#pragma once

#include "Bullet.hpp"
#include "game/objects/Object.hpp"
#include "game/objects/TempObjects.hpp"
#include "sound/ObjectSound.hpp"


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
    void Action(TvecBullet& vecBullet);
    Bullet* FireBullet(const PointF& pt);
    void Crash(TempObjects& vecObiekty);
    ObjectSound sndEngine;
    ObjectSound sndCrash;

protected:
    void OnRender() override;

private:
    Float CheckTime;
    Float CheckTimeElapsed;
    Float MoveTime;
    Float MoveTimeElapsed;
    Float FireTime;
    Float FireTimeElapsed;
};
