#include "Ufo.hpp"
#include <cmath>
#include "audio/Sound.hpp"
#include "game/Consts.hpp"
#include "game/GameConsts.hpp"
#include "game/Rand.hpp"
#include "game/Time.hpp"
#include "game/objects/AsterShards.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
Ufo::Ufo() : Object()
{
    geometryType = GeometryType::Polyg;
    scoreReward = GE_SCRVAL_UFO;
    setRandV(10.0f, 15.0f);
    setColor(GE_UFO_COLOR);

    verts.push_back(PointF(2.0f, 0.0f));
    verts.push_back(PointF(0.8f, -0.75f));
    verts.push_back(PointF(-0.8f, -0.75f));
    verts.push_back(PointF(-2.0f, 0.0f));
    verts.push_back(PointF(-0.8f, 0.75f));
    verts.push_back(PointF(0.8f, 0.75f));
    verts.push_back(PointF(2.0f, 0.0f));
    calcBounds(verts);

    sndEngine.init(SND_UFO_ENGINE, SND_VOL_UFO_ENGINE);
    sndCrash.init(SND_ASTER_CRASH2, SND_VOL_ASTER_CRASH2);
    sndEngine.play();
}

Ufo::~Ufo()
{
    sndEngine.stop();
}

void Ufo::update()
{
    Object::update();
}

void Ufo::onRender() const
{
    setGlColor(color);
    glBegin(GL_LINE_LOOP);
    for (auto it = verts.begin(); it != verts.end(); ++it)
        glVertex2d((*it).x, (*it).y);
    glEnd();
}

void Ufo::action(Bullets& bullets)
{
    const Float SafeDist = 12.0f;

    CheckTimeElapsed += time.dt;
    if (CheckTimeElapsed > CheckTime)
    {
        CheckTimeElapsed = 0.0f;
        Float RShp = 2e6f;
        Float RAst = 1e6f;
        if (pShip) RShp = distance(pShip);
        if (pAster) RAst = distance(pAster);
        if ((RShp < SafeDist) || (RAst < SafeDist)) MoveTimeElapsed = MoveTime;
    }

    MoveTimeElapsed += time.dt;
    if (MoveTimeElapsed > MoveTime)
    {
        Float RShp = 2e6f;
        Float RAst = 1e6f;
        if (pShip) RShp = distance(pShip);
        if (pAster) RAst = distance(pAster);
        if (pShip and RShp < SafeDist)
        {
            int sgn = RAND(2) ? -1 : 1;
            setVA(10.0f, pShip->getAlfa() + 90 * sgn);
        }
        if (pAster and RAst < SafeDist)
        {
            int sgn = RAND(2) ? -1 : 1;
            setVA(10.0f, pAster->getAlfa() + 90 * sgn);
        }
        else
        {
            setRandV(9.0f, 13.0f);
        }
        bullets.push_back(fireBullet({pos.x + getVX(), pos.y + getVY()})); // shoot in a velocity direction
        FireTimeElapsed = 0.0f;
        MoveTimeElapsed = 0.0f;
    }

    FireTimeElapsed += time.dt;
    if (FireTimeElapsed > FireTime)
    {
        Float RShp = 2e6f;
        Float RAst = 1e6f;
        if (pShip) RShp = distance(pShip);
        if (pAster) RAst = distance(pAster);
        if (RShp < RAst)
        {
            if (pShip and RShp < 35.0f)
            {
                bullets.push_back(fireBullet(pShip->pos));
            }
        }
        else
        {
            if (pAster and RAst < 20.0f)
            {
                bullets.push_back(fireBullet(pAster->pos));
            }
        }
        FireTimeElapsed = 0.0f;
    }
}

Bullet* Ufo::fireBullet(const PointF& pt)
{
    Float Speed = 22.0f;
    Bullet* bullet = new Bullet;
    bullet->lifeTime.interval = 3.0f;
    bullet->setXY(pos);
    Float alfa = std::atan2(pt.y - pos.y, pt.x - pos.x) * GE_180overPI;
    bullet->setAlfa(alfa + RAND(6) - 3.0f);
    bullet->setV(Speed);
    bullet->setColor(color);
    return bullet;
}

void Ufo::crash(TempObjects& vecObiekty)
{
    sndCrash.play();
    int iDebCount = GE_UFO_DEBR_COUNT;
    for (int i = 0; i < iDebCount; ++i)
    {
        AsterShards* pDeb = new AsterShards;
        pDeb->setColor(color);
        pDeb->setAlfa(getAlfa() + i * 360.0f / iDebCount + RAND(16) - 8.0f);
        pDeb->setXY(pos);
        const Float vRand = 3.0f + RAND(15);
        const Float vx = 0.8f * getVX() + vRand * std::cos(pDeb->getAlfa() * GE_PIover180);
        const Float vy = 0.8f * getVY() + vRand * std::sin(pDeb->getAlfa() * GE_PIover180);
        pDeb->setV(vx, vy);
        pDeb->setRotSpeed(rand() / 200 - 400.0f); // TODO: check this formula
        vecObiekty.push_back(pDeb);
    }
}
} // namespace game::objects
