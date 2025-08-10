#pragma once

#include <gl/gl.h>

namespace gl
{
class Color
{
public:
    constexpr Color(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f)
    {
        color[0] = r;
        color[1] = g;
        color[2] = b;
        color[3] = a;
    }
    operator const float*() const { return (float*)color; }

private:
    GLfloat color[4];
};

inline constexpr Color colorWhite{1.0f, 1.0f, 1.0f};
} // namespace gl
