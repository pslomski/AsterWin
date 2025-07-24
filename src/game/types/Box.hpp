#pragma once

#include "Types.hpp"

// TODO: make Box an aggregate type
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
    Box& operator=(const Box& other)
    {
        if (this != &other)
        {
            x0 = other.x0;
            y0 = other.y0;
            x1 = other.x1;
            y1 = other.y1;
        }
        return *this;
    }
    T x0, y0, x1, y1;
};

using BoxF = Box<Float>;
