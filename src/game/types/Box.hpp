#pragma once

#include "Types.hpp"

template <class T>
class Box
{
public:
    Box() {}
    Box(const Box& b)
    {
        x0 = b.x0;
        y0 = b.y0;
        x1 = b.x1;
        y1 = b.y1;
    }
    Box(T in_x0, T in_y0, T in_x1, T in_y1)
    {
        x0 = in_x0;
        y0 = in_y0;
        x1 = in_x1;
        y1 = in_y1;
    }
    T x0, y0, x1, y1;
};

using BoxF = Box<Float>;
