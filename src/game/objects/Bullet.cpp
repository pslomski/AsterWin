#include "Bullet.hpp"
#include "game/Consts.hpp"
#include "game/Rand.hpp"
#include "game/World.hpp"
#include "gl/Utils.hpp"

Bullet::Bullet() : TempObject()
{
    lifeTime.interval = 1.0;
    bounds.x0 = -0.1;
    bounds.x1 = 0.1;
    bounds.y0 = -0.1;
    bounds.y1 = 0.1;
}

void Bullet::OnRender()
{
    glPointSize(3.0);
    glBegin(GL_POINTS);
    setGlColor(color);
    glVertex2d(0.0, 0.0);
    glEnd();
}

TShipDebris::TShipDebris() : TempObject()
{
    m_Ratio = 1.0;
    Float D = 0.15 + 0.1 * RAND(2);
    lifeTime.interval = 1.3 + rand() % 9 * 0.1;
    bounds.x0 = -D;
    bounds.x1 = D;
    bounds.y0 = -D;
    bounds.y1 = D;
    setRotSpeed(720.0 + RAND(300));
}

void TShipDebris::update()
{
    Object::update();
    m_Ratio = 1.0 - lifeTime.ratio();
}

void TShipDebris::OnRender()
{
    setGlColor(color * m_Ratio);
    glBegin(GL_LINES);
    glVertex2d(bounds.x0, 0.0);
    glVertex2d(bounds.x1, 0.0);
    glEnd();
}

AsterDebris::AsterDebris() : TempObject()
{
    lifeTime.interval = 1.5 + rand() % 9 * 0.1;
    bounds.x0 = -0.1;
    bounds.x1 = 0.1;
    bounds.y0 = -0.1;
    bounds.y1 = 0.1;
}

void AsterDebris::update()
{
    Object::update();
    m_Ratio = 1.0 - lifeTime.ratio();
}

void AsterDebris::OnRender()
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

void StarBlink::OnRender()
{
    setGlColor(color * m_Ratio);
    glPointSize(GLfloat(m_Size));
    glBegin(GL_POINTS);
    glVertex2d(0.0, 0.0);
    glEnd();
}
