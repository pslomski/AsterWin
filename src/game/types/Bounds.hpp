#pragma once

#include <cassert> //TODO: Remove assert dependency from Bounds.hpp
#include "Types.hpp"

struct Bounds
{
    Float width()
    {
        assert(x1 >= x0);
        return x1 - x0;
    }
    Float height()
    {
        assert(y1 >= y0);
        return y1 - y0;
    }

    Float x0, x1, y0, y1;
};
