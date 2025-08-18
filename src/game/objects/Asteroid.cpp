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
    float degDelta{0.0f};
    float R{1.0f};
    if (level == 1)
    {
        scoreReward = GE_SCRVAL_ASTER1;
        R = 3.9f;
        degDelta = 36.0f;
        setRotSpeedDeg(rand(-60.0f, 60.0f));
        sndCrash.init(SND_ASTER_CRASH1, SND_VOL_ASTER_CRASH1);
    }
    else if (level == 2)
    {
        scoreReward = GE_SCRVAL_ASTER2;
        R = 2.8f;
        degDelta = 52.0f;
        setRotSpeedDeg(rand(-80.0f, 80.0f));
        sndCrash.init(SND_ASTER_CRASH2, SND_VOL_ASTER_CRASH2);
    }
    else if (level >= 3)
    {
        scoreReward = GE_SCRVAL_ASTER3;
        R = 1.7f;
        degDelta = 72.0f;
        setRotSpeedDeg(rand(-100.0f, 100.0f));
        sndCrash.init(SND_ASTER_CRASH3, SND_VOL_ASTER_CRASH3);
    }

    verts.clear();
    for (float deg = 0.0f; deg < 310.0f; deg += degDelta + rand(-9.0f, 9.0f))
    {
        PointF pt;
        float R2 = R * (0.7f + rand(0.0f, 0.3f));
        const auto angleRad = degToRad(deg);
        pt.x = R2 * std::cosf(angleRad);
        pt.y = R2 * std::sinf(angleRad);
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
    int rnd = rand(40);
    if (rnd < RandCount)
    {
        RandCount = 0;
        AsterCount = 0;
        BonusType bt = BonusType(rand(2));
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

    int debCount, asterCount;
    float lifeTime;
    BonusType BonusType = BonusType::None;
    if (level == 1)
    {
        asterCount = 2;
        debCount = GE_ASTER1_DEBR_COUNT;
        lifeTime = 1.1f + rand(0.0f, 0.7f);
    }
    else if (level == 2)
    {
        asterCount = 2;
        debCount = GE_ASTER2_DEBR_COUNT;
        lifeTime = 0.8f + rand(0.0f, 0.7f);
        BonusType = BonusType::Points;
    }
    else
    {
        asterCount = 0;
        debCount = GE_ASTER3_DEBR_COUNT;
        lifeTime = 0.5f + rand(0.0f, 0.7f);
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

    for (int i = 0; i < asterCount; ++i)
    {
        Asteroid* pAster = new Asteroid(level + 1);
        pAster->setAngleDeg(getAngleDeg() + i * 180.0f - rand(65.0f, 115.0f));
        const auto angleRad = pAster->getAngleRad();
        const auto x = state.pos.x + 3.0f * std::cosf(angleRad);
        const auto y = state.pos.y + 3.0f * std::sinf(angleRad);
        pAster->setPosition(x, y);
        pAster->setV(getV() * 1.3f);
        asteroids.push_back(pAster);
    }

    for (int i = 0; i < debCount; ++i)
    {
        AsterShards* pDeb = new AsterShards;
        pDeb->setAngleDeg(getAngleDeg() + i * 360.0f / debCount + rand(-8.0f, 8.0f));
        pDeb->setPosition(state.pos);
        pDeb->lifeTime.interval = lifeTime;
        const auto vRand = rand(5.0f, 20.0f);
        const auto angleRad = pDeb->getAngleRad();
        const auto vx = 0.8f * getVX() + vRand * std::cosf(angleRad);
        const auto vy = 0.8f * getVY() + vRand * std::sinf(angleRad);
        pDeb->setV(vx, vy);
        shards.push_back(pDeb);
    }
}
} // namespace game::objects
