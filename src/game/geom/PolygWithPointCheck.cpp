#include "PolygWithPointCheck.hpp"
#include <cmath>
#include "game/Consts.hpp"
#include "game/geom/LineIntersection.hpp"
#include "game/geom/PointInPolygon.hpp"

namespace
{
PointF rotate(PointF& pt, Float alfa)
{
    PointF res;
    Float sinalfa = std::sin(-alfa * GE_PIover180);
    Float cosalfa = std::cos(-alfa * GE_PIover180);
    res.x = pt.x * cosalfa + pt.y * sinalfa;
    res.y = pt.x * sinalfa + pt.y * cosalfa;
    return res;
}
} // namespace

namespace game::geom
{
bool checkPolygWithPoint(const objects::Object* point, const objects::Object* polygon)
{
    BoxF o1;
    Float x, y;
    // take the displacement vector of the point
    const BoxF o2{point->posp.x, point->posp.y, point->pos.x, point->pos.y};
    for (unsigned int i1 = 0; i1 < polygon->verts.size(); ++i1)
    {
        if (0 == i1)
        {
            o1 = polygon->transform(BoxF(
                polygon->verts[0].x,
                polygon->verts[0].y,
                polygon->verts[polygon->verts.size() - 1].x,
                polygon->verts[polygon->verts.size() - 1].y));
        }
        else
        {
            o1 = polygon->transform(
                BoxF{polygon->verts[i1 - 1].x, polygon->verts[i1 - 1].y, polygon->verts[i1].x, polygon->verts[i1].y});
        }
        if (linesIntersection(o1, o2, x, y) == 0)
        {
            return true;
        }
        else
        {
            PointF pt{point->pos.x - polygon->pos.x, point->pos.y - polygon->pos.y};
            pt = rotate(pt, -polygon->getAlfa());
            return isPointInPolygon(polygon->verts.size(), polygon->verts, pt.x, pt.y);
        }
    }
    return false;
}
} // namespace game::geom
