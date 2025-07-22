#include "game/objects/Object.hpp"
#include <cassert>
#include <gl/gl.h>
#include "game/Consts.hpp"
#include "game/World.hpp"
#include "game/geom/LineIntersection.hpp"
#include "game/geom/PolygWithPointCheck.hpp"

Float Object::dt = 0.0;

Object::Object()
{
    scoreReward = 0;
    geometryType = GeometryType::Point;
    glList = 0;
    setColor();
    angle = 0.0;
    omega = 0.0;
    setXY(0.0, 0.0);
    setA(0.0);
    setV(0.0);
    KDec = 0.0;
}

Object::~Object()
{
    if (glList)
    {
        glDeleteLists(glList, 1);
    }
}

Float Object::distance(const Object* object) const
{
    const auto dx = object->getX() - fx;
    const auto dy = object->getY() - fy;
    return sqrt(dx * dx + dy * dy);
}

void Object::setA(Float aa)
{
    fa = aa;
    fax = Float(fa * cos(angle * GE_PIover180));
    fay = Float(fa * sin(angle * GE_PIover180));
}

void Object::setV(Float av)
{
    fv = av;
    fvx = Float(fv * cos(angle * GE_PIover180));
    fvy = Float(fv * sin(angle * GE_PIover180));
}

void Object::setVA(Float av, Float alfa)
{
    fv = av;
    fvx = Float(fv * cos(alfa * GE_PIover180));
    fvy = Float(fv * sin(alfa * GE_PIover180));
}

void Object::setRandV(Float vmin, Float vmax)
{
    Float vRand = rand() % int(vmax - vmin) + vmin;
    Float alfa = rand() % 360;
    Float vx = vRand * cos(alfa * GE_PIover180);
    Float vy = vRand * sin(alfa * GE_PIover180);
    setV(vx, vy);
}

Float Object::correctAlfa(Float alfa)
{
    while (alfa < 0.0)
        alfa += 360.0;
    while (alfa > 360.0)
        alfa -= 360.0;
    return alfa;
}

void Object::move()
{
    xp = fx;
    yp = fy;
    alphap = angle;

    angle += omega * dt;
    if (abs(KDec) > 1e-6)
    {
        fvx += (fax - KDec * fvx * abs(fvx)) * dt;
        fvy += (fay - KDec * fvy * abs(fvy)) * dt;
    }
    else
    {
        fvx += fax * dt;
        fvy += fay * dt;
    }
    fx += fvx * dt;
    fy += fvy * dt;

    if (fx < geWorld.bounds.x0)
    {
        fx += geWorld.bounds.x1;
        xp += geWorld.bounds.x1;
    }
    if (fx > geWorld.bounds.x1)
    {
        fx -= geWorld.bounds.x1;
        xp -= geWorld.bounds.x1;
    }
    if (fy < geWorld.bounds.y0)
    {
        fy += geWorld.bounds.y1;
        yp += geWorld.bounds.y1;
    }
    if (fy > geWorld.bounds.y1)
    {
        fy -= geWorld.bounds.y1;
        yp -= geWorld.bounds.y1;
    }
}

BoxF Object::transform(const BoxF& seg) const
{
    BoxF res;
    Float sinalfa = sin(-angle * GE_PIover180);
    Float cosalfa = cos(-angle * GE_PIover180);
    res.x0 = fx + seg.x0 * cosalfa + seg.y0 * sinalfa;
    res.y0 = fy - seg.x0 * sinalfa + seg.y0 * cosalfa;
    res.x1 = fx + seg.x1 * cosalfa + seg.y1 * sinalfa;
    res.y1 = fy - seg.x1 * sinalfa + seg.y1 * cosalfa;
    return res;
}

bool Object::checkCollision(Object* pObiekt)
{
    assert(nullptr != pObiekt);

    if (((getX() + bounds.x0) > (pObiekt->getX() + pObiekt->bounds.x1)) ||
        ((getX() + bounds.x1) < (pObiekt->getX() + pObiekt->bounds.x0)) ||
        ((getY() + bounds.y0) > (pObiekt->getY() + pObiekt->bounds.y1)) ||
        ((getY() + bounds.y1) < (pObiekt->getY() + pObiekt->bounds.y0)))
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
                        o1 = transform(
                            BoxF(verts[0].x, verts[0].y, verts[verts.size() - 1].x, verts[verts.size() - 1].y));
                    else
                        o1 = transform(BoxF(verts[i1 - 1].x, verts[i1 - 1].y, verts[i1].x, verts[i1].y));
                    for (unsigned int i2 = 0; i2 < pObiekt->verts.size(); ++i2)
                    {
                        if (0 == i2)
                            o2 = pObiekt->transform(BoxF(
                                pObiekt->verts[0].x,
                                pObiekt->verts[0].y,
                                pObiekt->verts[pObiekt->verts.size() - 1].x,
                                pObiekt->verts[pObiekt->verts.size() - 1].y));
                        else
                            o2 = pObiekt->transform(BoxF(
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
                if (checkPolygWithPoint(pObiekt, this)) return true;
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
                if (checkPolygWithPoint(this, pObiekt)) return true;
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

void Object::draw()
{
    Float minterp = 1.0 - geWorld.interp;
    Float x = fx * geWorld.interp + xp * minterp;
    Float y = fy * geWorld.interp + yp * minterp;
    Float alfa = angle * geWorld.interp + alphap * minterp;
    glPushMatrix();
    glTranslated(x, y, 0.0);
    glRotated(alfa, 0.0, 0.0, 1.0);
    onRender();
    glPopMatrix();
}

void Object::render()
{
    onRender();
}

void Object::calcBounds(const PointsF& points)
{
    Float max = 0.0;
    for (const auto& point : points)
    {
        max = std::max(max, abs((point).x));
        max = std::max(max, abs((point).y));
    }
    bounds.x0 = -max;
    bounds.x1 = max;
    bounds.y0 = -max;
    bounds.y1 = max;
}
