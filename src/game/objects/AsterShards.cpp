#include "AsterShards.hpp"
#include <gl/gl.h>
#include "game/Rand.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
AsterShards::AsterShards() : TempObject()
{
    lifeTime.interval = 1.5f + 0.1f * RAND(9);
    bounds.x0 = -0.1f;
    bounds.x1 = 0.1f;
    bounds.y0 = -0.1f;
    bounds.y1 = 0.1f;
}

void AsterShards::update()
{
    TempObject::update();
    ratio = 1.0f - lifeTime.ratio();
}

void AsterShards::onRender() const
{
    setGlColor(color * ratio);
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
}
} // namespace game::objects
