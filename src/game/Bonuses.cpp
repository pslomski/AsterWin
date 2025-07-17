#include "Bonuses.hpp"
#include "GameConsts.hpp"
#include "audio/Sound.hpp"
#include "gl/Utils.hpp"

BonusObject::BonusObject(BonusType in_Type) : TempObject(GE_BONUS_LIFETIME), bonusType(in_Type)
{
    scoreReward = GE_SCRVAL_BONUS;
    tiRot.interval = 1.5;
    geometryType = GeometryType::Polyg;
    m_ColRatio = 1.0;

    Float D = 3.0;
    verts.push_back(PointF(-D, -D));
    verts.push_back(PointF(D, -D));
    verts.push_back(PointF(D, D));
    verts.push_back(PointF(-D, D));
    calcBounds(verts);
}

void BonusObject::update()
{
    TempObject::update();
    Float d = 0.75;
    m_ColRatio = lifeTime.ratio();
    if (m_ColRatio < d)
        m_ColRatio = 1.0;
    else
        m_ColRatio = (1 - m_ColRatio) / (1 - d);
    if (tiRot.inc(dt)) tiRot.reset();
}

void BonusObject::OnRender()
{
    glRotated(tiRot.ratio() * 360.0, 0.0, 1.0, 0.0);
    setGlColor(color * m_ColRatio);
    glCallList(glList);
}

////////////////////////////////////////////////////////////////////

TGEBonusBulletSpeed::TGEBonusBulletSpeed() : BonusObject(BonusType::BulletSpeed)
{
    Create();
}

void TGEBonusBulletSpeed::Create()
{
    Float D = 0.5;
    color = {GE_BONUS_BULLET_SPEED_COLOR};
    glList = glGenLists(1);
    glNewList(glList, GL_COMPILE);
    glBegin(GL_LINE_LOOP);
    glVertex2d(-D, 4 * D);
    glVertex2d(2 * D, 4 * D);
    glVertex2d(0.5 * D, D);
    glVertex2d(2.5 * D, 2 * D);
    glVertex2d(-D, -4 * D);
    glVertex2d(0.0, 0.0);
    glVertex2d(-2 * D, -D);
    glEnd();
    glEndList();
}

////////////////////////////////////////////////////////////////////

TGEBonusAddBullets::TGEBonusAddBullets() : BonusObject(BonusType::Bullets)
{
    Create();
}

void _DrawBullet()
{
    Float D = 1.0;
    glBegin(GL_LINE_LOOP);
    glVertex2d(0.0, 2.00 * D); // 1
    glVertex2d(0.25 * D, 1.75 * D); // 2
    glVertex2d(0.50 * D, 1.00 * D); // 3
    glVertex2d(0.50 * D, -1.25 * D); // 4
    glVertex2d(0.25 * D, -1.50 * D); // 5
    glVertex2d(0.50 * D, -1.75 * D); // 6
    glVertex2d(0.50 * D, -2.00 * D); // 7
    glVertex2d(-0.50 * D, -2.00 * D); // 8
    glVertex2d(-0.50 * D, -1.75 * D); // 9
    glVertex2d(-0.25 * D, -1.50 * D); // 10
    glVertex2d(-0.50 * D, -1.25 * D); // 11
    glVertex2d(-0.50 * D, 1.00 * D); // 12
    glVertex2d(-0.25 * D, 1.75 * D); // 13
    glEnd();
}

void TGEBonusAddBullets::Create()
{
    color = {GE_BONUS_ADD_BULLETS_COLOR};
    glList = glGenLists(1);
    glNewList(glList, GL_COMPILE);
    glRotated(31.0, 0.0, 0.0, 1.0);
    _DrawBullet();
    glEndList();
}

TGEBonusPoints::TGEBonusPoints() : BonusObject(BonusType::Points)
{
    scoreReward = GE_SCRVAL_BONUS;
    Create();
}

void TGEBonusPoints::Create()
{
    Float D = 0.25;
    color = {GE_BONUS_POINTS_COLOR};
    glList = glGenLists(1);
    glNewList(glList, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2d(2 * D, 3 * D);
    glVertex2d(1 * D, 4 * D);
    glVertex2d(-1 * D, 4 * D);
    glVertex2d(-2 * D, 3 * D);
    glVertex2d(-2 * D, 1 * D);
    glVertex2d(-1 * D, 0 * D);
    glVertex2d(1 * D, 0 * D);
    glVertex2d(2 * D, -1 * D);
    glVertex2d(2 * D, -3 * D);
    glVertex2d(1 * D, -4 * D);
    glVertex2d(-1 * D, -4 * D);
    glVertex2d(-2 * D, -3 * D);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2d(-0.5 * D, 5 * D);
    glVertex2d(-0.5 * D, -5 * D);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2d(0.5 * D, 5 * D);
    glVertex2d(0.5 * D, -5 * D);
    glEnd();
    glEndList();
}

////////////////////////////////////////////////////////////////////

BonusObject* geCreateBonusObj(BonusType in_Type)
{
    switch (in_Type)
    {
        case BonusType::Bullets:
            return new TGEBonusAddBullets;
        case BonusType::BulletSpeed:
            return new TGEBonusBulletSpeed;
        case BonusType::Points:
            return new TGEBonusPoints;
    }
    return new TGEBonusPoints;
}
