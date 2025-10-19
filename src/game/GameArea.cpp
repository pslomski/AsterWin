#include "GameArea.hpp"
#include "game/Rand.hpp"

namespace game
{
float GameArea::width() const
{
    return bounds.width();
}

float GameArea::height() const
{
    return bounds.height();
}

int GameArea::widthi() const
{
    return bounds.width();
}

int GameArea::heighti() const
{
    return bounds.height();
}

PointF GameArea::center() const
{
    return PointF((bounds.x0 + bounds.x1) * 0.5f, (bounds.y0 + bounds.y1) * 0.5f);
}

PointF GameArea::randomPosAtEdge() const
{
    constexpr int nbrOfSides = 4;
    switch (rand(nbrOfSides))
    {
        case 0:
            return {.x = bounds.x0, .y = rand(bounds.y0, bounds.y1)};
        case 1:
            return {.x = bounds.x1, .y = rand(bounds.y0, bounds.y1)};
        case 2:
            return {.x = rand(bounds.x0, bounds.x1), .y = bounds.y0};
        default:
            return {.x = rand(bounds.x0, bounds.x1), .y = bounds.y1};
    }
}
} // namespace game
