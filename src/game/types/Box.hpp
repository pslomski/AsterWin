#pragma once

#include "Types.hpp"

template <class T>
struct Box
{
    T x0;
    T y0;
    T x1;
    T y1;
};

using BoxF = Box<Float>;
