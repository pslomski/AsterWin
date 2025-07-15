#pragma once

#include <gl/gl.h>
#include "game/types/Color.hpp"

inline void setGlColor(const Color& color)
{
    glColor3f(color.red, color.green, color.blue);
}

inline void setGlColor(const float gray)
{
    glColor3f(gray, gray, gray);
}
