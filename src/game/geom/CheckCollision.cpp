#include "CheckCollision.hpp"
#include "game/geom/LineIntersection.hpp"
#include "game/geom/PolygWithPointCheck.hpp"
#include "game/geom/Transform.hpp"

namespace game::geom
{
bool checkCollision(const objects::Object* pObiekt1, const objects::Object* pObiekt)
{
    assert(nullptr != pObiekt);

    if (((pObiekt1->state.pos.x + pObiekt1->bounds.x0) > (pObiekt->state.pos.x + pObiekt->bounds.x1)) ||
        ((pObiekt1->state.pos.x + pObiekt1->bounds.x1) < (pObiekt->state.pos.x + pObiekt->bounds.x0)) ||
        ((pObiekt1->state.pos.y + pObiekt1->bounds.y0) > (pObiekt->state.pos.y + pObiekt->bounds.y1)) ||
        ((pObiekt1->state.pos.y + pObiekt1->bounds.y1) < (pObiekt->state.pos.y + pObiekt->bounds.y0)))
        return false;
    else
    {
        if (GeometryType::Polyg == pObiekt1->geometryType)
        {
            BoxF o1, o2;
            float _x, _y;
            if (GeometryType::Polyg == pObiekt->geometryType)
            {
                for (unsigned int i1 = 0; i1 < pObiekt1->verts.size(); ++i1)
                {
                    if (0 == i1)
                    {
                        o1 = geom::transform(
                            pObiekt1->getAngleRad(),
                            pObiekt1->state.pos,
                            BoxF(
                                pObiekt1->verts[0].x,
                                pObiekt1->verts[0].y,
                                pObiekt1->verts[pObiekt1->verts.size() - 1].x,
                                pObiekt1->verts[pObiekt1->verts.size() - 1].y));
                    }
                    else
                    {
                        o1 = geom::transform(
                            pObiekt1->getAngleRad(),
                            pObiekt1->state.pos,
                            BoxF(
                                pObiekt1->verts[i1 - 1].x,
                                pObiekt1->verts[i1 - 1].y,
                                pObiekt1->verts[i1].x,
                                pObiekt1->verts[i1].y));
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
                if (geom::checkPolygWithPoint(pObiekt, pObiekt1)) return true;
            }
            else
            {
                return true; // we assume intersection of Bounds as collision
            }
        }
        else if (GeometryType::Point == pObiekt1->geometryType)
        {
            if (GeometryType::Polyg == pObiekt->geometryType)
            {
                if (geom::checkPolygWithPoint(pObiekt1, pObiekt)) return true;
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
} // namespace game::geom
