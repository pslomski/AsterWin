#include "BonusAddBullets.hpp"
#include <gl/gl.h>
#include "game/GameConsts.hpp"

namespace
{
void drawBullet()
{
    constexpr auto d = 1.0f;
    glBegin(GL_LINE_LOOP);
    glVertex2f(0.0f, 2.00f * d); // 1
    glVertex2f(0.25f * d, 1.75f * d); // 2
    glVertex2f(0.50f * d, 1.00f * d); // 3
    glVertex2f(0.50f * d, -1.25f * d); // 4
    glVertex2f(0.25f * d, -1.50f * d); // 5
    glVertex2f(0.50f * d, -1.75f * d); // 6
    glVertex2f(0.50f * d, -2.00f * d); // 7
    glVertex2f(-0.50f * d, -2.00f * d); // 8
    glVertex2f(-0.50f * d, -1.75f * d); // 9
    glVertex2f(-0.25f * d, -1.50f * d); // 10
    glVertex2f(-0.50f * d, -1.25f * d); // 11
    glVertex2f(-0.50f * d, 1.00f * d); // 12
    glVertex2f(-0.25f * d, 1.75f * d); // 13
    glEnd();
}
} // namespace

namespace game::objects
{
BonusAddBullets::BonusAddBullets() : BonusObject(BonusType::Bullets)
{
    color = {GE_BONUS_ADD_BULLETS_COLOR};
    glList = glGenLists(1);
    glNewList(glList, GL_COMPILE);
    glRotatef(31.0f, 0.0f, 0.0f, 1.0f);
    drawBullet();
    glEndList();
}
} // namespace game::objects
