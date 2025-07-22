#include "Bonuses.hpp"
#include "game/GameConsts.hpp"
#include "game/objects/BonusAddBullets.hpp"
#include "game/objects/BonusBulletSpeed.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
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
            return new BonusAddBullets;
        case BonusType::BulletSpeed:
            return new BonusBulletSpeed;
        case BonusType::Points:
            return new TGEBonusPoints;
    }
    return new TGEBonusPoints;
}
} // namespace game::objects
