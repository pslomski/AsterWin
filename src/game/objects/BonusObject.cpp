#include "BonusObject.hpp"
#include "game/GameConsts.hpp"
#include "game/Time.hpp"
#include "game/geom/Bounds.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
BonusObject::BonusObject(const BonusType bonusTypeArg) : TempObject(GE_BONUS_LIFETIME), bonusType(bonusTypeArg)
{
    geometryType = GeometryType::Polyg;
    scoreReward = GE_SCRVAL_BONUS;

    constexpr auto d = 3.0f;
    verts.push_back(PointF(-d, -d));
    verts.push_back(PointF(d, -d));
    verts.push_back(PointF(d, d));
    verts.push_back(PointF(-d, d));
    bounds = geom::calcBounds(verts);
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
    if (tiRot.inc(time.dt))
    {
        tiRot.reset();
    }
}

void BonusObject::onRender() const
{
    glRotatef(tiRot.ratio() * 360.0f, 0.0f, 1.0f, 0.0f);
    setGlColor(color * colorRatio);
    glCallList(glList);
}
} // namespace game::objects
