#include "BonusObject.hpp"
#include "game/GameConsts.hpp"
#include "game/types/Point.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
BonusObject::BonusObject(const BonusType bonusTypeArg) : TempObject(GE_BONUS_LIFETIME), bonusType(bonusTypeArg)
{
    geometryType = GeometryType::Polyg;
    scoreReward = GE_SCRVAL_BONUS;
    tiRot.interval = 1.5f;

    constexpr auto d = 3.0f;
    verts.push_back(PointF(-d, -d));
    verts.push_back(PointF(d, -d));
    verts.push_back(PointF(d, d));
    verts.push_back(PointF(-d, d));
    calcBounds(verts);
}

void BonusObject::update()
{
    TempObject::update();

    constexpr auto d = 0.75f;
    colorRatio = lifeTime.ratio();
    if (colorRatio < d)
    {
        colorRatio = 1.0;
    }
    else
    {
        colorRatio = (1 - colorRatio) / (1 - d);
    }
    if (tiRot.inc(dt))
    {
        tiRot.reset();
    }
}

void BonusObject::onRender()
{
    glRotatef(tiRot.ratio() * 360.0f, 0.0f, 1.0f, 0.0f);
    setGlColor(color * colorRatio);
    glCallList(glList);
}
} // namespace game::objects
