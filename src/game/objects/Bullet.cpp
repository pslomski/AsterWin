#include "Bullet.hpp"
#include <gl/gl.h>
#include "gl/Utils.hpp"

namespace game::objects
{
Bullet::Bullet() : TempObject()
{
    lifeTime.interval = 1.0;
    bounds.x0 = -0.1;
    bounds.x1 = 0.1;
    bounds.y0 = -0.1;
    bounds.y1 = 0.1;
}

void Bullet::onRender()
{
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    setGlColor(color);
    glVertex2f(0.0f, 0.0f);
    glEnd();
}
} // namespace game::objects
