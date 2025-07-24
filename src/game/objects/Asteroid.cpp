#include "Asteroid.hpp"
#include <cmath>
#include "Bullet.hpp"
#include "audio/Sound.hpp"
#include "game/Consts.hpp"
#include "game/GameConsts.hpp"
#include "game/Rand.hpp"
#include "game/objects/AsterShards.hpp"
#include "game/rand.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
Asteroid::Asteroid(const int levelArg) : Object(), level{levelArg}
{
    geometryType = GeometryType::Polyg;
    Color(GE_COLOR_ASTER);
    create();
}

void Asteroid::create()
{
    Float DegDelta{0.0f};
    Float R{1.0f};
    if (level == 1)
    {
        scoreReward = GE_SCRVAL_ASTER1;
        R = 3.9;
        DegDelta = 36.0;
        setRotSpeed(RAND(60) - 120.0f);
        sndCrash.init(SND_ASTER_CRASH1, SND_VOL_ASTER_CRASH1);
    }
    else if (level == 2)
    {
        scoreReward = GE_SCRVAL_ASTER2;
        R = 2.8;
        DegDelta = 52.0;
        setRotSpeed(RAND(80) - 160.0f);
        sndCrash.init(SND_ASTER_CRASH2, SND_VOL_ASTER_CRASH2);
    }
    else if (level >= 3)
    {
        scoreReward = GE_SCRVAL_ASTER3;
        R = 1.7;
        DegDelta = 72.0;
        setRotSpeed(RAND(100) - 200.0f);
        sndCrash.init(SND_ASTER_CRASH3, SND_VOL_ASTER_CRASH3);
    }

    verts.clear();
    for (Float deg = 0.0; deg < 310.0; deg += DegDelta + (RAND(18) - 9))
    {
        PointF pt;
        Float R2 = R * (static_cast<Float>(0.7) + RAND(30) / 100.0);
        pt.x = R2 * std::cos(deg * GE_PIover180);
        pt.y = R2 * std::sin(deg * GE_PIover180);
        verts.push_back(pt);
    }
    calcBounds(verts);

    glList = glGenLists(1);
    glNewList(glList, GL_COMPILE);
    setGlColor(color);
    glBegin(GL_LINE_LOOP);
    for (auto it = verts.begin(); it != verts.end(); ++it)
    {
        glVertex2d((*it).x, (*it).y);
    }
    glEnd();
    glEndList();
}

void Asteroid::onRender()
{
    glCallList(glList);
}

BonusType getBonusType()
{
    static BonusType btLast = BonusType::None;
    static int RandCount = 0;
    static int AsterCount = 0;
    if (AsterCount < 5)
    {
        ++AsterCount;
        return BonusType::None;
    }
    int rnd = RAND(40);
    if (rnd < RandCount)
    {
        RandCount = 0;
        AsterCount = 0;
        BonusType bt = BonusType(RAND(2));
        if (bt == btLast)
        {
            switch (btLast)
            {
                case BonusType::Bullets:
                    bt = BonusType::BulletSpeed;
                    break;
                case BonusType::BulletSpeed:
                    bt = BonusType::Bullets;
                    break;
                default:
                    break;
            }
        }
        btLast = bt;
        return bt;
    }
    else
    {
        ++RandCount;
    }
    return BonusType::None;
}

void Asteroid::crash(Asteroids& asteroids, TempObjects& shards, Bonuses& bonuses, const bool canCreateBonus)
{
    sndCrash.play();

    int iDebCount, iAsterCount;
    Float LifeTime;
    BonusType BonusType = BonusType::None;
    if (level == 1)
    {
        iAsterCount = 2;
        iDebCount = GE_ASTER1_DEBR_COUNT;
        LifeTime = 1.1 + rand() % 7 * 0.1;
    }
    else if (level == 2)
    {
        iAsterCount = 2;
        iDebCount = GE_ASTER2_DEBR_COUNT;
        LifeTime = 0.8 + rand() % 7 * 0.1;
        BonusType = BonusType::Points;
    }
    else
    {
        iAsterCount = 0;
        iDebCount = GE_ASTER3_DEBR_COUNT;
        LifeTime = 0.5 + rand() % 7 * 0.1;
        BonusType = getBonusType();
    }

    if (canCreateBonus and (BonusType not_eq BonusType::None))
    {
        BonusObject* bonus = createBonusObj(BonusType);
        if (bonus)
        {
            bonus->setXY(getX(), getY());
            bonus->setRandV(2.0, 3.0);
            bonuses.push_back(bonus);
        }
    }

    for (int i = 0; i < iAsterCount; ++i)
    {
        Asteroid* pAster = new Asteroid(level + 1);
        pAster->setAlfa(getAlfa() + i * 180.0 - 90.0 + rand() % 50 - 25.0);
        Float x = getX() + 3.0f * std::cos(pAster->getAlfa() * GE_PIover180);
        Float y = getY() + 3.0f * std::sin(pAster->getAlfa() * GE_PIover180);
        pAster->setXY(x, y);
        pAster->setV(getV() * 1.3);
        asteroids.push_back(pAster);
    }

    for (int i = 0; i < iDebCount; ++i)
    {
        AsterShards* pDeb = new AsterShards;
        pDeb->setAlfa(getAlfa() + i * 360.0 / iDebCount + rand() % 16 - 8.0);
        pDeb->setXY(getX(), getY());
        pDeb->lifeTime.interval = LifeTime;
        Float vRand = 5.0f + rand() % 15;
        Float vx = 0.8f * getVX() + vRand * std::cos(pDeb->getAlfa() * GE_PIover180);
        Float vy = 0.8f * getVY() + vRand * std::sin(pDeb->getAlfa() * GE_PIover180);
        pDeb->setV(vx, vy);
        shards.push_back(pDeb);
    }
}
} // namespace game::objects
