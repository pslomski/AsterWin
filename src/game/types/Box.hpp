#pragma once

#include <cassert>
#include "Types.hpp"

template <class T>
struct Box
{
    T width() const
    {
        assert(x1 >= x0);
        return x1 - x0;
    }

    T height() const
    {
        assert(y1 >= y0);
        return y1 - y0;
    }

    T x0;
    T y0;
    T x1;
    T y1;
};

using BoxF = Box<Float>;
