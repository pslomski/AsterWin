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
    PointF pt;
    int iSide = rand(4);
    if (0 == iSide)
    {
        pt.x = bounds.x0;
        pt.y = rand(static_cast<int>(bounds.height()));
    }
    else if (1 == iSide)
    {
        pt.x = bounds.x1;
        pt.y = rand(static_cast<int>(bounds.height()));
    }
    else if (2 == iSide)
    {
        pt.x = rand(static_cast<int>(bounds.width()));
        pt.y = bounds.y0;
    }
    else
    {
        pt.x = rand(static_cast<int>(bounds.width()));
        pt.y = bounds.y1;
    }
    return pt;
}
} // namespace game
