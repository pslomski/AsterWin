#pragma once

namespace ui
{
template <class T>
class Rectangle
{
public:
    Rectangle() : top(0), bottom(0), left(0), right(0) {}
    Rectangle(T aTop, T aBottom, T aLeft, T aRight) : top(aTop), bottom(aBottom), left(aLeft), right(aRight) {}
    ~Rectangle() {}

    void set(T aTop, T aBottom, T aLeft, T aRight)
    {
        top = aTop;
        bottom = aBottom;
        left = aLeft;
        right = aRight;
    }
    T getWidth() const { return right - left; }
    T getHeight() const { return bottom - top; }

    bool intersect(const Rectangle& rect) const
    {
        return (left < rect.right) and (right > rect.left) and (bottom > rect.top) and (top < rect.bottom);
    }

    bool isValid() const
    {
        if ((top > bottom) or (left > right)) return false;
        return true;
    }

    void offsetRect(int xOffset, int yOffset)
    {
        top += yOffset;
        bottom += yOffset;
        left += xOffset;
        right += xOffset;
    }

    T top;
    T bottom;
    T left;
    T right;
};
// Typdefs for the most used types.
typedef Rectangle<int> Rectanglei;
typedef Rectangle<float> Rectanglef;
typedef Rectangle<double> Rectangled;
} // namespace ui
