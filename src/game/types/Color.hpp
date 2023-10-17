#pragma once

class Color
{
public:
    Color() { Set(); }

    Color(const float red, const float green, const float blue) { Set(red, green, blue); }

    Color(const Color& col)
    {
        red = col.red;
        green = col.green;
        blue = col.blue;
    }

    void set(const Color& col)
    {
        red = col.red;
        green = col.green;
        blue = col.blue;
    }

    void Set(const float red = 1.0, const float green = 1.0, const float blue = 1.0)
    {
        this->red = red;
        this->green = green;
        this->blue = blue;
    }

    Color& operator=(const Color& color)
    {
        set(color);
        return *this;
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
