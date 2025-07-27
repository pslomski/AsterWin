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
    setColor();
    bounds.x0 = -0.1f;
    bounds.x1 = 0.1f;
    bounds.y0 = -0.1f;
    bounds.y1 = 0.1f;
    init();
}

void StarBlink::init()
{
    size = 1.0f + RAND(2);
    lifeTime.reset();
    lifeTime.interval = 2.0f + 1.0f * RAND(30);
    setXY(RAND(gameArea.widthi()), RAND(gameArea.heighti()));
}

void StarBlink::update()
{
    if (expired())
    {
        init();
    }

    TempObject::update();
    ratio = std::sin(lifeTime.ratio() * GE_PI);
}

void StarBlink::onRender() const
{
    setGlColor(color * ratio);
    glPointSize(GLfloat(size));
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glEnd();
}
} // namespace game::objects
