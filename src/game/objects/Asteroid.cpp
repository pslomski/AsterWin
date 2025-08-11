#include "Asteroid.hpp"
#include <cmath>
#include "audio/Sound.hpp"
#include "game/Consts.hpp"
#include "game/GameConsts.hpp"
#include "game/Rand.hpp"
#include "game/geom/Bounds.hpp"
#include "game/objects/AsterShards.hpp"
#include "game/rand.hpp"
#include "game/types/Color.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
Asteroid::Asteroid(const int levelArg) : Object(), level{levelArg}
{
    geometryType = GeometryType::Polyg;
    setColor(colorAster);
    create();
}

void Asteroid::create()
{
    Float degDelta{0.0f};
    Float R{1.0f};
    if (level == 1)
    {
        scoreReward = GE_SCRVAL_ASTER1;
        R = 3.9f;
        degDelta = 36.0f;
        setRotSpeedDeg(randi(60) - 120.0f);
        sndCrash.init(SND_ASTER_CRASH1, SND_VOL_ASTER_CRASH1);
    }
    else if (level == 2)
    {
        scoreReward = GE_SCRVAL_ASTER2;
        R = 2.8f;
        degDelta = 52.0f;
        setRotSpeedDeg(randi(80) - 160.0f);
        sndCrash.init(SND_ASTER_CRASH2, SND_VOL_ASTER_CRASH2);
    }
    else if (level >= 3)
    {
        scoreReward = GE_SCRVAL_ASTER3;
        R = 1.7f;
        degDelta = 72.0f;
        setRotSpeedDeg(randi(100) - 200.0f);
        sndCrash.init(SND_ASTER_CRASH3, SND_VOL_ASTER_CRASH3);
    }

    verts.clear();
    for (Float deg = 0.0f; deg < 310.0f; deg += degDelta + (randi(18) - 9))
    {
        PointF pt;
        Float R2 = R * (0.7f + randi(30) / 100.0f);
        const auto angleRad = degToRad(deg);
        pt.x = R2 * std::cos(angleRad);
        pt.y = R2 * std::sin(angleRad);
        verts.push_back(pt);
    }
    bounds = geom::calcBounds(verts);

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

void Asteroid::onRender() const
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
    int rnd = randi(40);
    if (rnd < RandCount)
    {
        RandCount = 0;
        AsterCount = 0;
        BonusType bt = BonusType(randi(2));
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
        LifeTime = 1.1f + randi(7) * 0.1f;
    }
    else if (level == 2)
    {
        iAsterCount = 2;
        iDebCount = GE_ASTER2_DEBR_COUNT;
        LifeTime = 0.8f + randi(7) * 0.1F;
        BonusType = BonusType::Points;
    }
    else
    {
        iAsterCount = 0;
        iDebCount = GE_ASTER3_DEBR_COUNT;
        LifeTime = 0.5f + randi(7) * 0.1f;
        BonusType = getBonusType();
    }

    if (canCreateBonus and (BonusType not_eq BonusType::None))
    {
        BonusObject* bonus = createBonusObj(BonusType);
        if (bonus)
        {
            bonus->setPosition(state.pos);
            bonus->setRandV(2.0f, 3.0f);
            bonuses.push_back(bonus);
        }
    }

    for (int i = 0; i < iAsterCount; ++i)
    {
        Asteroid* pAster = new Asteroid(level + 1);
        pAster->setAngleDeg(getAngleDeg() + i * 180.0f - 90.0f + randi(50) - 25.0f);
        const auto angleRad = pAster->getAngleRad();
        Float x = state.pos.x + 3.0f * std::cos(angleRad);
        Float y = state.pos.y + 3.0f * std::sin(angleRad);
        pAster->setPosition(x, y);
        pAster->setV(getV() * 1.3);
        asteroids.push_back(pAster);
    }

    for (int i = 0; i < iDebCount; ++i)
    {
        AsterShards* pDeb = new AsterShards;
        pDeb->setAngleDeg(getAngleDeg() + i * 360.0f / iDebCount + randi(16) - 8.0f);
        pDeb->setPosition(state.pos);
        pDeb->lifeTime.interval = LifeTime;
        Float vRand = 5.0f + randi(15);
        const auto angleRad = pDeb->getAngleRad();
        Float vx = 0.8f * getVX() + vRand * std::cos(angleRad);
        Float vy = 0.8f * getVY() + vRand * std::sin(angleRad);
        pDeb->setV(vx, vy);
        shards.push_back(pDeb);
    }
}
} // namespace game::objects
