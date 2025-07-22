#include "BonusObject.hpp"
#include "game/GameConsts.hpp"
#include "gl/Utils.hpp"

namespace game::objects
{
BonusObject::BonusObject(BonusType in_Type) : TempObject(GE_BONUS_LIFETIME), bonusType(in_Type)
{
    scoreReward = GE_SCRVAL_BONUS;
    tiRot.interval = 1.5;
    geometryType = GeometryType::Polyg;
    m_ColRatio = 1.0;

    Float D = 3.0;
    verts.push_back(PointF(-D, -D));
    verts.push_back(PointF(D, -D));
    verts.push_back(PointF(D, D));
    verts.push_back(PointF(-D, D));
    calcBounds(verts);
}

void BonusObject::update()
{
    TempObject::update();
    Float d = 0.75;
    m_ColRatio = lifeTime.ratio();
    if (m_ColRatio < d)
        m_ColRatio = 1.0;
    else
        m_ColRatio = (1 - m_ColRatio) / (1 - d);
    if (tiRot.inc(dt)) tiRot.reset();
}

void BonusObject::onRender()
{
    glRotated(tiRot.ratio() * 360.0, 0.0, 1.0, 0.0);
    setGlColor(color * m_ColRatio);
    glCallList(glList);
}
} // namespace game::objects
