#include "LineIntersection.hpp"
#include <cmath>

// intersection point of segments
// result
// 0 - segments intersect; x, y contain the intersection point
// 1 - intersection lies on o1 but outside o2; x, y contain the intersection point
// 2 - intersection lies on o2 but outside o1; x, y contain the intersection point
// 3 - lines intersect but outside the segments; x, y contain the intersection point
// 4 - segments are parallel
int linesIntersection(const BoxF& o1, const BoxF& o2, float& x, float& y)
{
    float ua, ub;
    float la, lb, m; // numerator and denominator
    m = (o2.y1 - o2.y0) * (o1.x1 - o1.x0) - (o2.x1 - o2.x0) * (o1.y1 - o1.y0);
    if (std::fabs(m) < 1e-6)
    {
        // if denominator=0 then lines are parallel
        return 4;
    }
    la = (o2.x1 - o2.x0) * (o1.y0 - o2.y0) - (o2.y1 - o2.y0) * (o1.x0 - o2.x0);
    lb = (o1.x1 - o1.x0) * (o1.y0 - o2.y0) - (o1.y1 - o1.y0) * (o1.x0 - o2.x0);

    ua = la / m;
    x = o1.x0 + ua * (o1.x1 - o1.x0);
    y = o1.y0 + ua * (o1.y1 - o1.y0);
    ub = lb / m;
    if ((ua >= 0) && (ua <= 1) && (ub >= 0) && (ub <= 1))
        return 0;
    else if ((ua >= 0) && (ua <= 1) && ((ub < 0) || (ub > 1)))
        return 1;
    else if (((ua < 0) || (ua > 1)) && (ub >= 0) && (ub <= 1))
        return 2;
    else
        return 3;
}
