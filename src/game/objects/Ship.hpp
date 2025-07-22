#pragma once

#include <gl/gl.h>
#include <math.h>
#include <windows.h>
#include "Bullet.hpp"
#include "audio/SfxSample.hpp"
#include "game/objects/Object.hpp"
#include "game/objects/PowerUp.hpp"
#include "game/objects/PowerUpAddBullets.hpp"
#include "game/objects/PowerUpBulletSpeed.hpp"
#include "game/objects/Ship.fwd.hpp"
#include "game/objects/TempObjects.hpp"
#include "game/types/BonusType.hpp"
#include "utils/TimeInterval.hpp"

namespace game::objects
{
class Ship : public Object
{
private:
    Color m_clrTmp;
    Float m_BoostLen;
    Float Accel, AccelBurst, AccelMax;
    Float m_RotSpeed;
    Float m_RespBlinkColRatio;
    bool faccelerated;
    bool fshield;
    bool bDarken; // okresowe przyciemnianie koloru
    Float sndEngineGain;
    bool EngSndStopped;
    Color clEngine;
    utils::TimeInterval m_tiAccel;
    utils::TimeInterval m_tiFade;
    utils::TimeInterval m_tiEngineBlink;
    utils::TimeInterval m_tiRotateLeft, m_tiRotateRight;
    utils::TimeInterval m_tiRespawnBlink;
    utils::TimeInterval m_tiRespawn;

    void onRender() override;

public:
    Ship(Float ax, Float ay, Float aangle);
    ~Ship();

    void update() override;
    Float BulletSpeed;
    bool Respawning;
    unsigned int MaxBullets;
    void AccelerationOn();
    void AccelerationOff();
    void RotateLeft();
    void RotateRight();
    void RotateLeftStop() { m_tiRotateLeft.reset(); }
    void RotateRightStop() { m_tiRotateRight.reset(); }
    Bullet* FireBullet();
    void Crash(TempObjects& vecObiekty);
    void Respawn();
    void AddBonus(BonusType type);
    PowerUpAddBullets puAddBullet; // PowerUp dodatkowe strzaly
    PowerUpBulletSpeed puBulletSpeed; // PowerUp zwiekszona predkosc pociskow
    SfxSample sndFire;
    SfxSample sndFirePow;
    SfxSample sndEngine;
    SfxSample sndCrash;
};
} // namespace game::objects
