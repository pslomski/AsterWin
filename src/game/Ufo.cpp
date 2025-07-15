#include "Ufo.hpp"
#include "Consts.hpp"
#include "GameConsts.hpp"
#include "Sound.hpp"
#include "utils/GlUtils.hpp"

Ufo::Ufo() : Object()
{
    sndEngine.Init(SND_UFO_ENGINE, SND_VOL_UFO_ENGINE);
    sndCrash.Init(SND_ASTER_CRASH2, SND_VOL_ASTER_CRASH2);
    scoreReward = GE_SCRVAL_UFO;
    geometryType = GeometryType::Polyg;
    pShip = nullptr;
    pAster = nullptr;
    CheckTime = 0.7;
    CheckTimeElapsed = 0.0;
    MoveTime = 3.0;
    MoveTimeElapsed = 0.0;
    FireTime = 2.0;
    FireTimeElapsed = 0.0;
    setColor(GE_UFO_COLOR);
    setRandV(10.0, 15.0);

    verts.push_back(PointF(2.0, 0.0));
    verts.push_back(PointF(0.8, -0.75));
    verts.push_back(PointF(-0.8, -0.75));
    verts.push_back(PointF(-2.0, 0.0));
    verts.push_back(PointF(-0.8, 0.75));
    verts.push_back(PointF(0.8, 0.75));
    verts.push_back(PointF(2.0, 0.0));
    calcBounds(verts);

    sndEngine.Play();
}

Ufo::~Ufo()
{
    sndEngine.Stop();
}

void Ufo::update()
{
    Object::update();
}

void Ufo::OnRender()
{
    setGlColor(color);
    glBegin(GL_LINE_LOOP);
    for (auto it = verts.begin(); it != verts.end(); ++it)
        glVertex2d((*it).x, (*it).y);
    glEnd();
}

void Ufo::Action(TvecBullet& vecBullet)
{
    const Float SafeDist = 12.0;

    CheckTimeElapsed += dt;
    if (CheckTimeElapsed > CheckTime)
    {
        CheckTimeElapsed = 0.0;
        Float RShp = 2e6;
        Float RAst = 1e6;
        if (pShip) RShp = distance(pShip);
        if (pAster) RAst = distance(pAster);
        if ((RShp < SafeDist) || (RAst < SafeDist)) MoveTimeElapsed = MoveTime;
    }

    MoveTimeElapsed += dt;
    if (MoveTimeElapsed > MoveTime)
    {
        Float RShp = 2e6;
        Float RAst = 1e6;
        if (pShip) RShp = distance(pShip);
        if (pAster) RAst = distance(pAster);
        if (pShip && RShp < SafeDist)
        {
            int sgn = rand() % 2 ? -1 : 1;
            setVA(10.0, pShip->getAlfa() + 90 * sgn);
        }
        if (pAster && RAst < SafeDist)
        {
            int sgn = rand() % 2 ? -1 : 1;
            setVA(10.0, pAster->getAlfa() + 90 * sgn);
        }
        else
        {
            setRandV(9.0, 13.0);
        }
        Float x = getX() + getVX();
        Float y = getY() + getVY();
        vecBullet.push_back(FireBullet(PointF(x, y))); // prewencyjny strzal w nowym kierunku ruchu
        FireTimeElapsed = 0.0;
        MoveTimeElapsed = 0.0;
    }

    FireTimeElapsed += dt;
    if (FireTimeElapsed > FireTime)
    {
        Float RShp = 2e6;
        Float RAst = 1e6;
        if (pShip) RShp = distance(pShip);
        if (pAster) RAst = distance(pAster);
        if (RShp < RAst)
        {
            if (pShip && RShp < 35.0)
            {
                vecBullet.push_back(FireBullet(pShip->getXY()));
            }
        }
        else
        {
            if (pAster && RAst < 20.0)
            {
                vecBullet.push_back(FireBullet(pAster->getXY()));
            }
        }
        FireTimeElapsed = 0.0;
    }
}

Bullet* Ufo::FireBullet(const PointF& pt)
{
    Float Speed = 22.0;
    Bullet* bullet = new Bullet;
    bullet->lifeTime.interval = 3.0;
    bullet->setXY(getX(), getY());
    Float alfa = atan2(pt.y - getY(), pt.x - getX()) * GE_180overPI;
    bullet->setAlfa(alfa + rand() % 6 - 3);
    bullet->setV(Speed);
    bullet->setColor(color);
    return bullet;
}

void Ufo::Crash(TempObjects& vecObiekty)
{
    sndCrash.Play();
    int iDebCount = GE_UFO_DEBR_COUNT;
    for (int i = 0; i < iDebCount; ++i)
    {
        AsterDebris* pDeb = new AsterDebris;
        pDeb->setColor(color);
        pDeb->setAlfa(getAlfa() + i * 360.0 / iDebCount + rand() % 16 - 8.0);
        pDeb->setXY(getX(), getY());
        Float vRand = 3.0 + rand() % 15;
        Float vx = 0.8 * getVX() + vRand * cos(pDeb->getAlfa() * GE_PIover180);
        Float vy = 0.8 * getVY() + vRand * sin(pDeb->getAlfa() * GE_PIover180);
        pDeb->setV(vx, vy);
        pDeb->setRotSpeed(rand() / 200 - 400);
        vecObiekty.push_back(pDeb);
    }
}
