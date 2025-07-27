#include "PointInPolygon.hpp"

// nvert - number of vertices of the bounding polygon
// verts - bounding polygon (the last point must be different from the first)
// testx, testy - point to test
// result:
// true - is inside the polygon
// false - is outside the polygon
bool isPointInPolygon(const int nvert, const PointsF& verts, const Float testx, const Float testy)
{
    bool c = false;
    int i, j = 0;
    for (i = 0, j = nvert - 1; i < nvert; j = i++)
    {
        const auto& pti{verts[i]};
        const auto& ptj{verts[j]};
        if (((pti.y > testy) != (ptj.y > testy)) and
            (testx < (ptj.x - pti.x) * (testy - pti.y) / (ptj.y - pti.y) + pti.x))
            c = !c;
    }
    return c;
}
