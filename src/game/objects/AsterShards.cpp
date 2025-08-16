#include "AsterShards.hpp"
#include <gl/gl.h>
#include "game/Rand.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
AsterShards::AsterShards() : TempObject()
{
    lifeTime.interval = rand(1.5f, 2.3f);
    constexpr auto r{0.1f};
    bounds = {.x0 = -r, .y0 = -r, .x1 = r, .y1 = r};
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
