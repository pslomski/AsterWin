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

inline constexpr Color colorWhite{1.0f, 1.0f, 1.0f};
inline constexpr Color colorAster{1.0f, 1.0f, 1.0f};
inline constexpr Color colorUfo{0.6f, 0.9f, 0.2f};
inline constexpr Color colorShip{1.0f, 1.0f, 0.0f};
inline constexpr Color colorShipEngine{0.9f, 0.2f, 0.0f};
inline constexpr Color colorBonusBulletSpeed{0.2f, 0.7f, 1.0f};
inline constexpr Color colorBonusAddBullets{0.9f, 0.1f, 0.1f};
inline constexpr Color colorBonusPoints{1.0f, 0.6f, 0.0f};
