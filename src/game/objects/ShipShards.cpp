#include "ShipShards.hpp"
#include "game/Rand.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
ShipShards::ShipShards() : TempObject()
{
    const auto d = 0.15f + 0.1f * randi(2);
    lifeTime.interval = 1.3f + randi(9) * 0.1f;
    bounds = {.x0 = -d, .y0 = -d, .x1 = d, .y1 = d};
    setRotSpeedDeg(720.0f + randi(300));
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
