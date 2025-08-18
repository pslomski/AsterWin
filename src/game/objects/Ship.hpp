#pragma once

#include <gl/gl.h>
#include <math.h>
#include <windows.h>
#include "Bullet.hpp"
#include "audio/SfxSample.hpp"
#include "game/GameConsts.hpp"
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
public:
    Ship(const float xArg, const float yArg, const float angleArg);
    ~Ship();

    void update() override;
    void accelerationOn();
    void accelerationOff();
    void rotateLeft();
    void rotateRight();
    void rotateLeftStop() { tiRotateLeft.reset(); }
    void rotateRightStop() { tiRotateRight.reset(); }
    Bullet* fireBullet();
    void crash(TempObjects& vecObiekty);
    void respawn();
    void addBonus(const BonusType type);

    bool respawning{false};
    unsigned int maxBullets{GE_INITIAL_MAX_BULLETS};
    float bulletSpeed{GE_INITIAL_BULLET_SPEED};
    SfxSample sndEngine;

private:
    void onRender() const override;

    PowerUpAddBullets puAddBullet;
    PowerUpBulletSpeed puBulletSpeed;
    Color colorCurr;
    float boostLength{};
    float accel{20.0f}; // ~2g
    float accelBurst{15.0f};
    float accelMax{accel + accelBurst};
    float rotationSpeedDeg{GE_SHIP_ROT_SPEED};
    bool faccelerated{false};
    float respBlinkColRatio{1.0f};
    bool bDarken{false}; // temporary darkening of the ship
    Color clEngine;
    utils::TimeInterval tiAccel{0.5f};
    utils::TimeInterval tiEngineBlink{0.25f};
    utils::TimeInterval tiRotateLeft{0.5f};
    utils::TimeInterval tiRotateRight{0.5f};
    utils::TimeInterval tiRespawnBlink{0.3f};
    utils::TimeInterval tiRespawn{3.0f};
    float sndEngineGain{};
    SfxSample sndFire;
    SfxSample sndFirePow;
    SfxSample sndCrash;
};
} // namespace game::objects
