#pragma once

#include <gl/gl.h>
#include <math.h>
#include <windows.h>
#include "Bullet.hpp"
#include "Object.hpp"
#include "ObjectSound.hpp"
#include "types/BonusType.hpp"
#include "types/Color.hpp"
#include "utils/TimeInterval.hpp"
#include "utils/Tools.hpp"

class Ship;

class PowerUp
{
public:
    PowerUp(Float in_Dutation);
    utils::TimeInterval Duration;
    void Start();
    void Stop();
    bool isActive() { return m_bActive; }

protected:
    virtual void OnStart() = 0;
    virtual void OnStop() = 0;
    bool m_bActive;
};

class AddBulletPowerUp : public PowerUp
{
public:
    AddBulletPowerUp(Float in_Dutation) : PowerUp(in_Dutation){};
    Ship* pShip;

protected:
    void OnStart();
    void OnStop();
};

class BulletSpeedPowerUp : public PowerUp
{
public:
    BulletSpeedPowerUp(Float in_Dutation) : PowerUp(in_Dutation){};
    Ship* pShip;

protected:
    void OnStart();
    void OnStop();
};

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

protected:
    void OnRender() override;

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
    void RotateLeftStop() { m_tiRotateLeft.Reset(); }
    void RotateRightStop() { m_tiRotateRight.Reset(); }
    Bullet* FireBullet();
    void Crash(ObjectList& vecObiekty);
    void Respawn();
    void AddBonus(BonusType type);
    AddBulletPowerUp puAddBullet; // PowerUp dodatkowe strzaly
    BulletSpeedPowerUp puBulletSpeed; // PowerUp zwiekszona predkosc pociskow
    ObjectSound sndFire;
    ObjectSound sndFirePow;
    ObjectSound sndEngine;
    ObjectSound sndCrash;
};
