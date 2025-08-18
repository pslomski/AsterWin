#pragma once

#include <vector>

template <class T>
struct Point
{
    T x;
    T y;
};

using PointF = Point<float>;
using PointsF = std::vector<PointF>;
