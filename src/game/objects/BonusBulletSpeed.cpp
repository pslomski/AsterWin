#include "BonusBulletSpeed.hpp"
#include <gl/gl.h>
#include "game/GameConsts.hpp"

namespace game::objects
{
BonusBulletSpeed::BonusBulletSpeed() : BonusObject(BonusType::BulletSpeed)
{
    color = {GE_BONUS_BULLET_SPEED_COLOR};
    constexpr auto d = 0.5f;
    glList = glGenLists(1);
    glNewList(glList, GL_COMPILE);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-d, 4 * d);
    glVertex2f(2 * d, 4 * d);
    glVertex2f(0.5 * d, d);
    glVertex2f(2.5 * d, 2 * d);
    glVertex2f(-d, -4 * d);
    glVertex2f(0.0, 0.0);
    glVertex2f(-2 * d, -d);
    glEnd();
    glEndList();
}
} // namespace game::objects
