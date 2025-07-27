#pragma once

struct Color
{
    void set(const Color& col)
    {
        red = col.red;
        green = col.green;
        blue = col.blue;
    }

    Color operator*(const float k)
    {
        Color col = *this;
        col.red *= k;
        col.green *= k;
        col.blue *= k;
        return col;
    }

    float red{1.0f};
    float green{1.0f};
    float blue{1.0f};
};
