#include "Ship.hpp"
#include <algorithm>
#include <cmath>
#include "audio/Sound.hpp"
#include "game/Consts.hpp"
#include "game/Time.hpp"
#include "game/objects/AsterShards.hpp"
#include "game/objects/ShipShards.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
Ship::Ship(const Float xArg, const Float yArg, const Float angleArg)
    : Object(), puAddBullet(GE_POWERUP_DURATION_TIME), puBulletSpeed(GE_POWERUP_DURATION_TIME)
{
    geometryType = GeometryType::Polyg;
    friction = 0.05f;
    setXY(xArg, yArg);
    setAlfa(angleArg);
    setRotSpeed(0.0f);
    setColor(GE_SHIP_COLOR);

    puAddBullet.pShip = this;
    puBulletSpeed.pShip = this;
    clEngine.set({GE_SHIP_ENGINE_COLOR});
    verts.push_back(PointF(2.0f, 0.0f));
    verts.push_back(PointF(-1.5f, -1.0f));
    verts.push_back(PointF(-1.0f, 0.0f));
    verts.push_back(PointF(-1.5f, 1.0f));
    calcBounds(verts);

    sndFire.init(SND_SHIP_FIRE, SND_VOL_SHIP_FIRE);
    sndFirePow.init(SND_SHIP_FIRE_POWER, SND_VOL_SHIP_FIRE_POWER);
    sndEngine.init(SND_SHIP_ENGINE, SND_VOL_SHIP_ENGINE);
    sndCrash.init(SND_SHIP_CRASH, SND_VOL_SHIP_CRASH);
}

Ship::~Ship()
{
    sndEngine.stop();
}

void Ship::update()
{
    Object::update();
    const auto dt{time.dt};
    if (puAddBullet.duration.inc(dt)) puAddBullet.stop();
    if (puBulletSpeed.duration.inc(dt)) puBulletSpeed.stop();

    if (respawning)
    {
        if (tiRespawnBlink.inc(dt))
        {
            tiRespawnBlink.reset();
            bDarken = !bDarken;
        }
        Float d = bDarken ? 0.5f : 1.0f;
        respBlinkColRatio = 0.5f * (1.0f + tiRespawnBlink.ratio()) * d;
    }
    else
    {
        respBlinkColRatio = 1.0f;
    }

    if (faccelerated)
    {
        if (tiEngineBlink.inc(dt)) tiEngineBlink.reset();
    }

    if (faccelerated)
    {
        boostLength = getA() / accelBurst * 2.0 + 0.25 * sin(tiEngineBlink.ratio() * 2 * GE_PI);
    }

    // ustalanie koloru
    colorCurr = color;
    if (puAddBullet.isActive())
    {
        Float alfa = 2 * GE_PI * puAddBullet.duration.elapsed;
        Float sina = std::sin(alfa);
        sina *= sina;
        Float cosa = std::cos(alfa);
        cosa *= cosa;
        Color cl1{GE_BONUS_ADD_BULLETS_COLOR};
        cl1 = cl1 * sina;
        Color cl2{colorCurr};
        cl2 = cl2 * cosa;
        colorCurr.red = std::min(1.0f, cl1.red + cl2.red);
        colorCurr.green = std::min(1.0f, cl1.green + cl2.green);
        colorCurr.blue = std::min(1.0f, cl1.blue + cl2.blue);
    }
    if (puBulletSpeed.isActive())
    {
        Float alfa = 2 * GE_PI * puBulletSpeed.duration.elapsed;
        Float sina = std::sin(alfa);
        sina *= sina;
        Float cosa = std::cos(alfa);
        cosa *= cosa;
        Color cl1{GE_BONUS_BULLET_SPEED_COLOR};
        cl1 = cl1 * sina;
        Color cl2{colorCurr};
        cl2 = cl2 * cosa;
        colorCurr.red = std::min(1.0f, cl1.red + cl2.red);
        colorCurr.green = std::min(1.0f, cl1.green + cl2.green);
        colorCurr.blue = std::min(1.0f, cl1.blue + cl2.blue);
    }
}

void Ship::onRender() const
{
    if (faccelerated)
    {
        setGlColor(clEngine * respBlinkColRatio);
        glBegin(GL_LINE_STRIP);
        glVertex2d(-1.5, -1);
        glVertex2d(-boostLength, 0);
        glVertex2d(-1.5, 1);
        glEnd();
    }

    setGlColor(colorCurr * respBlinkColRatio);
    glBegin(GL_LINE_LOOP);
    for (auto it = verts.begin(); it != verts.end(); ++it)
        glVertex2d((*it).x, (*it).y);
    glEnd();
}

void Ship::accelerationOn()
{
    if (tiAccel.inc(time.dt)) tiAccel.elapsed = tiAccel.interval;
    setA(accel + (1.0 - tiAccel.ratio()) * accelBurst);
    sndEngineGain = SND_VOL_SHIP_ENGINE * getA() / accelMax;
    sndEngine.setPitch(float(getA() / accelMax));
    if (!faccelerated)
    {
        sndEngine.setVolume(float(sndEngineGain));
        sndEngine.play();
    }
    faccelerated = true;
}

void Ship::accelerationOff()
{
    if (faccelerated)
    {
        sndEngine.slideVol(0.0f, 100);
    }

    tiAccel.reset();
    setA(0.0);
    faccelerated = false;
}

void Ship::rotateLeft()
{
    tiRotateLeft.inc(time.dt);
    setAlfa(getAlfa() + std::min(0.5f * (1.0f + tiRotateLeft.ratio()), 1.0f) * rotationSpeed * time.dt);
}

void Ship::rotateRight()
{
    tiRotateRight.inc(time.dt);
    setAlfa(getAlfa() - std::min(0.5f * (1.0f + tiRotateRight.ratio()), 1.0f) * rotationSpeed * time.dt);
}

Bullet* Ship::fireBullet()
{
    if (puAddBullet.isActive() || puBulletSpeed.isActive())
        sndFirePow.play();
    else
        sndFire.play();

    Bullet* bullet = new Bullet;
    bullet->setXY(pos);
    bullet->setAlfa(getAlfa());
    Float vx = getVX() + bulletSpeed * std::cos(getAlfa() * GE_PIover180);
    Float vy = getVY() + bulletSpeed * std::sin(getAlfa() * GE_PIover180);
    bullet->setV(vx, vy);
    bullet->setColor(color);
    return bullet;
}

void Ship::crash(TempObjects& vecObiekty)
{
    sndCrash.play();

    int iDebCount = GE_SHIP_LIN_DEBR_COUNT;
    for (int i = 0; i < iDebCount; ++i)
    {
        AsterShards* pDeb = new AsterShards;
        pDeb->setColor(color);
        pDeb->setAlfa(getAlfa() + i * 360.0 / iDebCount + rand() % 16 - 8.0);
        pDeb->setXY(pos);
        Float vRand = 15.0 + rand() % 5;
        Float vx = getVX() + vRand * std::cos(pDeb->getAlfa() * GE_PIover180);
        Float vy = getVY() + vRand * std::sin(pDeb->getAlfa() * GE_PIover180);
        pDeb->setV(vx, vy);
        vecObiekty.push_back(pDeb);
    }

    iDebCount = GE_SHIP_DOT_DEBR_COUNT;
    for (int i = 0; i < iDebCount; ++i)
    {
        ShipShards* pDeb = new ShipShards;
        pDeb->setColor(color);
        pDeb->setAlfa(getAlfa() + i * 360.0 / iDebCount + rand() % 16 - 8.0);
        pDeb->setXY(pos);
        Float vRand = 3.0 + rand() % 15;
        Float vx = 0.8 * getVX() + vRand * std::cos(pDeb->getAlfa() * GE_PIover180);
        Float vy = 0.8 * getVY() + vRand * std::sin(pDeb->getAlfa() * GE_PIover180);
        pDeb->setV(vx, vy);
        vecObiekty.push_back(pDeb);
    }
}

void Ship::respawn()
{
    if (tiRespawn.inc(time.dt))
    {
        tiRespawn.reset();
        respawning = false;
    }
    else
    {
        respawning = true;
    }
}

void Ship::addBonus(const BonusType type)
{
    switch (type)
    {
        case BonusType::Bullets:
            puAddBullet.start();
            break;
        case BonusType::BulletSpeed:
            puBulletSpeed.start();
            break;
        default:
            break;
    }
}
} // namespace game::objects
