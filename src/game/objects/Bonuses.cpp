#include "Bonuses.hpp"
#include "game/GameConsts.hpp"
#include "game/objects/BonusBulletSpeed.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
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

game::objects::BonusObject* geCreateBonusObj(BonusType in_Type)
{
    switch (in_Type)
    {
        case BonusType::Bullets:
            return new TGEBonusAddBullets;
        case BonusType::BulletSpeed:
            return new BonusBulletSpeed;
        case BonusType::Points:
            return new TGEBonusPoints;
    }
    return new TGEBonusPoints;
}
} // namespace game::objects
