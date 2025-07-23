#include "Bullet.hpp"
#include "game/Consts.hpp"
#include "game/Rand.hpp"
#include "game/World.hpp"
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

AsterShards::AsterShards() : TempObject()
{
    lifeTime.interval = 1.5 + rand() % 9 * 0.1;
    bounds.x0 = -0.1;
    bounds.x1 = 0.1;
    bounds.y0 = -0.1;
    bounds.y1 = 0.1;
}

void AsterShards::update()
{
    TempObject::update();
    m_Ratio = 1.0 - lifeTime.ratio();
}

void AsterShards::onRender()
{
    setGlColor(color * m_Ratio);
    glPointSize(2.0);
    glBegin(GL_POINTS);
    glVertex2d(0.0, 0.0);
    glEnd();
}

StarBlink::StarBlink() : TempObject()
{
    setColor();
    bounds.x0 = -0.1;
    bounds.x1 = 0.1;
    bounds.y0 = -0.1;
    bounds.y1 = 0.1;
    Init();
}

void StarBlink::Init()
{
    m_Size = RAND(2) + 1;
    lifeTime.reset();
    lifeTime.interval = 2.0 + RAND(30) * 0.1;
    setXY(RAND(int(geWorld.getWidth())), RAND(int(geWorld.getHeight())));
}

void StarBlink::update()
{
    if (expired())
    {
        Init();
    };

    Object::update();
    m_Ratio = sin(lifeTime.ratio() * GE_PI);
}

void StarBlink::onRender()
{
    setGlColor(color * m_Ratio);
    glPointSize(GLfloat(m_Size));
    glBegin(GL_POINTS);
    glVertex2d(0.0, 0.0);
    glEnd();
}
} // namespace game::objects
