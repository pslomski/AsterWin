#include "Object.hpp"
#include <cassert>
#include <cmath>
#include <gl/gl.h>
#include "game/GameArea.hpp"
#include "game/Rand.hpp"
#include "game/Time.hpp"
#include "game/geom/LineIntersection.hpp"
#include "game/geom/PolygWithPointCheck.hpp"
#include "game/geom/Transform.hpp"
#include "game/types/Box.hpp"

namespace game::objects
{
Object::~Object()
{
    if (glList)
    {
        glDeleteLists(glList, 1);
    }
}

void Object::setA(const Float aa)
{
    fa = aa;
    a.x = fa * std::cos(state.angleRad);
    a.y = fa * std::sin(state.angleRad);
}

void Object::setV(const Float av)
{
    v.x = av * std::cos(state.angleRad);
    v.y = av * std::sin(state.angleRad);
}

void Object::setVA(const Float av, const Float angleRad)
{
    v.x = av * std::cos(angleRad);
    v.y = av * std::sin(angleRad);
}

void Object::setRandV(const Float vmin, const Float vmax)
{
    const Float vRand = randi(vmax - vmin) + vmin;
    const auto angleRad = degToRad(randi(360));
    Float vx = vRand * std::cos(angleRad);
    Float vy = vRand * std::sin(angleRad);
    setV(vx, vy);
}

Float Object::getV() const
{
    return std::sqrt(v.x * v.x + v.y * v.y);
}

void Object::move()
{
    statep = state;

    state.angleRad += omegaRad * time.dt;
    if (std::abs(friction) > 1e-6)
    {
        v.x += (a.x - friction * v.x * std::abs(v.x)) * time.dt;
        v.y += (a.y - friction * v.y * std::abs(v.y)) * time.dt;
    }
    else
    {
        v.x += a.x * time.dt;
        v.y += a.y * time.dt;
    }
    state.pos.x += v.x * time.dt;
    state.pos.y += v.y * time.dt;

    if (state.pos.x < gameArea.bounds.x0)
    {
        state.pos.x += gameArea.bounds.x1;
        statep.pos.x += gameArea.bounds.x1;
    }
    if (state.pos.x > gameArea.bounds.x1)
    {
        state.pos.x -= gameArea.bounds.x1;
        statep.pos.x -= gameArea.bounds.x1;
    }
    if (state.pos.y < gameArea.bounds.y0)
    {
        state.pos.y += gameArea.bounds.y1;
        statep.pos.y += gameArea.bounds.y1;
    }
    if (state.pos.y > gameArea.bounds.y1)
    {
        state.pos.y -= gameArea.bounds.y1;
        statep.pos.y -= gameArea.bounds.y1;
    }
}

bool Object::checkCollision(Object* pObiekt)
{
    assert(nullptr != pObiekt);

    if (((state.pos.x + bounds.x0) > (pObiekt->state.pos.x + pObiekt->bounds.x1)) ||
        ((state.pos.x + bounds.x1) < (pObiekt->state.pos.x + pObiekt->bounds.x0)) ||
        ((state.pos.y + bounds.y0) > (pObiekt->state.pos.y + pObiekt->bounds.y1)) ||
        ((state.pos.y + bounds.y1) < (pObiekt->state.pos.y + pObiekt->bounds.y0)))
        return false;
    else
    {
        if (GeometryType::Polyg == geometryType)
        {
            BoxF o1, o2;
            Float _x, _y;
            if (GeometryType::Polyg == pObiekt->geometryType)
            {
                for (unsigned int i1 = 0; i1 < verts.size(); ++i1)
                {
                    if (0 == i1)
                    {
                        o1 = geom::transform(
                            getAngleRad(),
                            state.pos,
                            BoxF(verts[0].x, verts[0].y, verts[verts.size() - 1].x, verts[verts.size() - 1].y));
                    }
                    else
                    {
                        o1 = geom::transform(
                            getAngleRad(), state.pos, BoxF(verts[i1 - 1].x, verts[i1 - 1].y, verts[i1].x, verts[i1].y));
                    }
                    for (unsigned int i2 = 0; i2 < pObiekt->verts.size(); ++i2)
                    {
                        if (0 == i2)
                            o2 = geom::transform(
                                pObiekt->getAngleRad(),
                                pObiekt->state.pos,
                                BoxF(
                                    pObiekt->verts[0].x,
                                    pObiekt->verts[0].y,
                                    pObiekt->verts[pObiekt->verts.size() - 1].x,
                                    pObiekt->verts[pObiekt->verts.size() - 1].y));
                        else
                            o2 = geom::transform(
                                pObiekt->getAngleRad(),
                                pObiekt->state.pos,
                                BoxF(
                                    pObiekt->verts[i2 - 1].x,
                                    pObiekt->verts[i2 - 1].y,
                                    pObiekt->verts[i2].x,
                                    pObiekt->verts[i2].y));
                        if (linesIntersection(o1, o2, _x, _y) == 0) return true;
                    }
                }
            }
            else if (GeometryType::Point == pObiekt->geometryType)
            {
                if (geom::checkPolygWithPoint(pObiekt, this)) return true;
            }
            else
            {
                return true; // we assume intersection of Bounds as collision
            }
        }
        else if (GeometryType::Point == geometryType)
        {
            if (GeometryType::Polyg == pObiekt->geometryType)
            {
                if (geom::checkPolygWithPoint(this, pObiekt)) return true;
            }
            else
            {
                return true; // in the case of 2 points, we assume intersection of Bounds as collision
            }
        }
        else
        {
            return true; // we assume intersection of Bounds as collision
        }
        return false;
    }
}

void Object::draw() const
{
    // State state = currentState * interp + previousState * (1.0 - interp);
    const auto minterp = 1.0f - interp;
    const auto x = state.pos.x * interp + statep.pos.x * minterp;
    const auto y = state.pos.y * interp + statep.pos.y * minterp;
    const auto angle = state.angleRad * interp + statep.angleRad * minterp;
    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glRotatef(radToDeg(angle), 0.0f, 0.0f, 1.0f);
    onRender();
    glPopMatrix();
}

void Object::render()
{
    onRender();
}

float Object::interp = 0.0;
} // namespace game::objects
