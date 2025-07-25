#include "Ship.hpp"
#include <algorithm>
#include "audio/Sound.hpp"
#include "game/Consts.hpp"
#include "game/GameConsts.hpp"
#include "game/Time.hpp"
#include "game/objects/AsterShards.hpp"
#include "game/objects/ShipShards.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
Ship::Ship(Float ax, Float ay, Float aangle)
    : Object(), puAddBullet(GE_POWERUP_DURATION_TIME), puBulletSpeed(GE_POWERUP_DURATION_TIME)
{
    sndFire.init(SND_SHIP_FIRE, SND_VOL_SHIP_FIRE);
    sndFirePow.init(SND_SHIP_FIRE_POWER, SND_VOL_SHIP_FIRE_POWER);
    sndEngine.init(SND_SHIP_ENGINE, SND_VOL_SHIP_ENGINE);
    sndCrash.init(SND_SHIP_CRASH, SND_VOL_SHIP_CRASH);

    m_tiAccel.interval = 0.5;
    m_tiRespawn.interval = 3.0;
    m_tiFade.interval = 0.15;
    m_tiEngineBlink.interval = 0.25;
    m_tiRotateLeft.interval = 0.25;
    m_tiRotateRight.interval = 0.25;
    m_tiRespawnBlink.interval = 0.3;
    m_RespBlinkColRatio = 1.0;

    puAddBullet.pShip = this;
    puBulletSpeed.pShip = this;
    BulletSpeed = GE_INITIAL_BULLET_SPEED;
    MaxBullets = GE_INITIAL_MAX_BULLETS;
    geometryType = GeometryType::Polyg;
    setColor(GE_SHIP_COLOR);
    clEngine.Set(GE_SHIP_ENGINE_COLOR);
    m_RotSpeed = GE_SHIP_ROT_SPEED;
    setRotSpeed(0.0);
    setXY(ax, ay);
    setAlfa(aangle);
    fshield = false;
    bDarken = false;
    faccelerated = false;
    Respawning = false;
    KDec = 0.05;
    Accel = 20.0; // ok. 2g
    AccelBurst = 15.0;
    AccelMax = Accel + AccelBurst; // pomocnicza przy wyswietlaniu plomienia
    sndEngineGain = 0.0;
    EngSndStopped = true;

    verts.push_back(PointF(2.0, 0.0));
    verts.push_back(PointF(-1.5, -1.0));
    verts.push_back(PointF(-1.0, 0.0));
    verts.push_back(PointF(-1.5, 1.0));
    calcBounds(verts);
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

    if (Respawning)
    {
        if (m_tiRespawnBlink.inc(dt))
        {
            m_tiRespawnBlink.reset();
            bDarken = !bDarken;
        }
        Float d = bDarken ? 0.5 : 1.0;
        m_RespBlinkColRatio = 0.5 * (1 + m_tiRespawnBlink.ratio()) * d;
    }
    else
    {
        m_RespBlinkColRatio = 1.0;
    }

    if (faccelerated)
    {
        if (m_tiEngineBlink.inc(dt)) m_tiEngineBlink.reset();
    }

    if (faccelerated)
    {
        m_BoostLen = getA() / AccelBurst * 2.0 + 0.25 * sin(m_tiEngineBlink.ratio() * 2 * GE_PI);
    }

    // ustalanie koloru
    m_clrTmp = color;
    if (puAddBullet.isActive())
    {
        Float alfa = 2 * GE_PI * puAddBullet.duration.elapsed;
        Float sina = sin(alfa);
        sina *= sina;
        Float cosa = cos(alfa);
        cosa *= cosa;
        Color cl1(GE_BONUS_ADD_BULLETS_COLOR);
        cl1 = cl1 * sina;
        Color cl2(m_clrTmp);
        cl2 = cl2 * cosa;
        m_clrTmp.red = std::min(1.0f, cl1.red + cl2.red);
        m_clrTmp.green = std::min(1.0f, cl1.green + cl2.green);
        m_clrTmp.blue = std::min(1.0f, cl1.blue + cl2.blue);
    }
    if (puBulletSpeed.isActive())
    {
        Float alfa = 2 * GE_PI * puBulletSpeed.duration.elapsed;
        Float sina = sin(alfa);
        sina *= sina;
        Float cosa = cos(alfa);
        cosa *= cosa;
        Color cl1(GE_BONUS_BULLET_SPEED_COLOR);
        cl1 = cl1 * sina;
        Color cl2(m_clrTmp);
        cl2 = cl2 * cosa;
        m_clrTmp.red = std::min(1.0f, cl1.red + cl2.red);
        m_clrTmp.green = std::min(1.0f, cl1.green + cl2.green);
        m_clrTmp.blue = std::min(1.0f, cl1.blue + cl2.blue);
    }
}

void Ship::onRender()
{
    if (faccelerated)
    {
        setGlColor(clEngine * m_RespBlinkColRatio);
        glBegin(GL_LINE_STRIP);
        glVertex2d(-1.5, -1);
        glVertex2d(-m_BoostLen, 0);
        glVertex2d(-1.5, 1);
        glEnd();
    }

    setGlColor(m_clrTmp * m_RespBlinkColRatio);
    glBegin(GL_LINE_LOOP);
    for (auto it = verts.begin(); it != verts.end(); ++it)
        glVertex2d((*it).x, (*it).y);
    glEnd();
}

void Ship::AccelerationOn()
{
    if (m_tiAccel.inc(time.dt)) m_tiAccel.elapsed = m_tiAccel.interval;
    setA(Accel + (1.0 - m_tiAccel.ratio()) * AccelBurst);
    sndEngineGain = SND_VOL_SHIP_ENGINE * getA() / AccelMax;
    sndEngine.setPitch(float(getA() / AccelMax));
    if (!faccelerated)
    {
        sndEngine.setVolume(float(sndEngineGain));
        sndEngine.play();
        EngSndStopped = false;
    }
    faccelerated = true;
    m_tiFade.reset();
}

void Ship::AccelerationOff()
{
    if (faccelerated)
    {
        sndEngine.slideVol(0.0f, 100);
    }

    m_tiAccel.reset();
    setA(0.0);
    faccelerated = false;
}

void Ship::RotateLeft()
{
    m_tiRotateLeft.inc(time.dt);
    setAlfa(getAlfa() + std::min(0.5 * (1.0 + m_tiRotateLeft.ratio()), 1.0) * m_RotSpeed * time.dt);
}

void Ship::RotateRight()
{
    m_tiRotateRight.inc(time.dt);
    setAlfa(getAlfa() - std::min(0.5 * (1.0 + m_tiRotateRight.ratio()), 1.0) * m_RotSpeed * time.dt);
}

Bullet* Ship::FireBullet()
{
    if (puAddBullet.isActive() || puBulletSpeed.isActive())
        sndFirePow.play();
    else
        sndFire.play();

    Bullet* bullet = new Bullet;
    bullet->setXY(getX(), getY());
    bullet->setAlfa(getAlfa());
    Float vx = getVX() + BulletSpeed * cos(getAlfa() * GE_PIover180);
    Float vy = getVY() + BulletSpeed * sin(getAlfa() * GE_PIover180);
    bullet->setV(vx, vy);
    bullet->setColor(color);
    return bullet;
}

void Ship::Crash(TempObjects& vecObiekty)
{
    sndCrash.play();

    int iDebCount = GE_SHIP_LIN_DEBR_COUNT;
    for (int i = 0; i < iDebCount; ++i)
    {
        AsterShards* pDeb = new AsterShards;
        pDeb->setColor(color);
        pDeb->setAlfa(getAlfa() + i * 360.0 / iDebCount + rand() % 16 - 8.0);
        pDeb->setXY(getX(), getY());
        Float vRand = 15.0 + rand() % 5;
        Float vx = getVX() + vRand * cos(pDeb->getAlfa() * GE_PIover180);
        Float vy = getVY() + vRand * sin(pDeb->getAlfa() * GE_PIover180);
        pDeb->setV(vx, vy);
        vecObiekty.push_back(pDeb);
    }

    iDebCount = GE_SHIP_DOT_DEBR_COUNT;
    for (int i = 0; i < iDebCount; ++i)
    {
        ShipShards* pDeb = new ShipShards;
        pDeb->setColor(color);
        pDeb->setAlfa(getAlfa() + i * 360.0 / iDebCount + rand() % 16 - 8.0);
        pDeb->setXY(getX(), getY());
        Float vRand = 3.0 + rand() % 15;
        Float vx = 0.8 * getVX() + vRand * cos(pDeb->getAlfa() * GE_PIover180);
        Float vy = 0.8 * getVY() + vRand * sin(pDeb->getAlfa() * GE_PIover180);
        pDeb->setV(vx, vy);
        vecObiekty.push_back(pDeb);
    }
}

void Ship::Respawn()
{
    if (m_tiRespawn.inc(time.dt))
    {
        m_tiRespawn.reset();
        Respawning = false;
    }
    else
    {
        Respawning = true;
    }
}

void Ship::AddBonus(const BonusType type)
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
