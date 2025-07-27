#pragma once

#include <vector>
#include "Types.hpp"

template <class T>
struct Point
{
    T x;
    T y;
};

using PointF = Point<Float>;
using PointsF = std::vector<PointF>;
