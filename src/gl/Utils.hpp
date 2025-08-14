#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockUtils.hpp"
#if USE_ORIG(gl_Utils)

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

bool setVSync(const int interval = 1);
void setGlView(const int width, const int height, const bool topDown = true);

#endif // USE_ORIG(gl_Utils)
