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
    glPointSize(3.0);
    glBegin(GL_POINTS);
    setGlColor(color);
    glVertex2d(0.0, 0.0);
    glEnd();
}
} // namespace game::objects
