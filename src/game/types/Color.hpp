#pragma once

struct Color
{
    void set(const Color& col)
    {
        red = col.red;
        green = col.green;
        blue = col.blue;
    }

    Color operator*(const float k) const { return {.red = red * k, .green = green * k, .blue = blue * k}; }

    float red{1.0f};
    float green{1.0f};
    float blue{1.0f};
};
