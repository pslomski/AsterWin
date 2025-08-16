#include "ShipShards.hpp"
#include "game/Rand.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
ShipShards::ShipShards() : TempObject()
{
    lifeTime.interval = rand(1.3f, 2.1f);
    const auto d = 0.1f;
    bounds = {.x0 = -d, .y0 = -d, .x1 = d, .y1 = d};
    setRotSpeedDeg(rand(720.f, 1020.0f));
}

void ShipShards::update()
{
    TempObject::update();
    ratio = 1.0f - lifeTime.ratio();
}

void ShipShards::onRender() const
{
    setGlColor(color * ratio);
    glBegin(GL_LINES);
    glVertex2f(bounds.x0, 0.0f);
    glVertex2f(bounds.x1, 0.0f);
    glEnd();
}
} // namespace game::objects
