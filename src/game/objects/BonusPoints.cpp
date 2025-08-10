#include "BonusPoints.hpp"
#include "game/GameConsts.hpp"
#include "game/types/Color.hpp"

namespace game::objects
{
BonusPoints::BonusPoints() : BonusObject(BonusType::Points)
{
    scoreReward = GE_SCRVAL_BONUS;
    color = colorBonusPoints;
    constexpr auto d = 0.25f;
    glList = glGenLists(1);
    glNewList(glList, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    glVertex2f(2.0f * d, 3.0f * d);
    glVertex2f(1.0f * d, 4.0f * d);
    glVertex2f(-1.0f * d, 4.0f * d);
    glVertex2f(-2.0f * d, 3.0f * d);
    glVertex2f(-2.0f * d, 1.0f * d);
    glVertex2f(-1.0f * d, 0.0f * d);
    glVertex2f(1.0f * d, 0.0f * d);
    glVertex2f(2.0f * d, -1.0f * d);
    glVertex2f(2.0f * d, -3.0f * d);
    glVertex2f(1.0f * d, -4.0f * d);
    glVertex2f(-1.0f * d, -4.0f * d);
    glVertex2f(-2.0f * d, -3.0f * d);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(-0.5f * d, 5.0f * d);
    glVertex2f(-0.5f * d, -5.0f * d);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glVertex2f(0.5f * d, 5.0f * d);
    glVertex2f(0.5f * d, -5.0f * d);
    glEnd();
    glEndList();
}
} // namespace game::objects
