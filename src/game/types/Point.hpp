#pragma once

#include <vector>
#include "Types.hpp"

template <class T>
class Point
{
public:
    Point() {}
    Point(const Point& p)
    {
        x = p.x;
        y = p.y;
    }
    Point(const T in_x, const T in_y)
    {
        x = in_x;
        y = in_y;
    }
    T x, y;
};

using PointF = Point<Float>;
using PointsF = std::vector<PointF>;
