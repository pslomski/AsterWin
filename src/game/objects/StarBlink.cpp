#include "StarBlink.hpp"
#include <cmath>
#include <gl/gl.h>
#include "game/Consts.hpp"
#include "game/GameArea.hpp"
#include "game/Rand.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
StarBlink::StarBlink() : TempObject()
{
    constexpr float d{0.1f};
    bounds = {.x0 = -d, .y0 = -d, .x1 = d, .y1 = d};
    setColor(colorWhite);
    init();
}

void StarBlink::init()
{
    size = 1.0f + randi(2);
    lifeTime.reset(2.0f + 0.1f * randi(20));
    setPosition(randi(gameArea.widthi()), randi(gameArea.heighti()));
}

void StarBlink::update()
{
    if (expired())
    {
        init();
    }

    TempObject::update();
    ratio = std::sinf(lifeTime.ratio() * pi);
}

void StarBlink::onRender() const
{
    setGlColor(color * ratio);
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
}
} // namespace game::objects
