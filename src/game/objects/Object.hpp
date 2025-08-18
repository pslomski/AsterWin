#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockObject.hpp"
#if USE_ORIG(game_objects_Object)

#include <gl/gl.h>
#include "game/Math.hpp"
#include "game/objects/Object.fwd.hpp"
#include "game/objects/ObjectState.hpp"
#include "game/types/Bounds.hpp"
#include "game/types/Color.hpp"
#include "game/types/ObjectGeom.hpp"
#include "game/types/Point.hpp"
#include "game/types/Position.hpp"
#include "game/types/Vector.hpp"

namespace game::objects
{
class Object
{
public:
    virtual ~Object();

    void move();
    void setPosition(const float x, const float y) { setPosition({x, y}); }
    void setPosition(const PointF& pt)
    {
        state.pos = pt;
        statep.pos = pt;
    }
    void setV(const float avx, const float avy)
    {
        v.x = avx;
        v.y = avy;
    }
    void setV(const float av);
    void setVA(const float av, const float angleRad);
    void setRandV(const float vmin, const float vmax);
    float getVX() const { return v.x; }
    float getVY() const { return v.y; }
    float getV() const;
    void setA(const float aa);
    float getA() const { return fa; }
    void setAngleRad(const float angleRadArg)
    {
        state.angleRad = normalizeAngleRad(angleRadArg);
        statep.angleRad = state.angleRad;
    }
    void setAngleDeg(const float angleDegArg) { setAngleRad(degToRad(angleDegArg)); }
    float getAngleDeg() const { return radToDeg(state.angleRad); }
    float getAngleRad() const { return state.angleRad; }
    void setRotSpeedRad(const float omegaRadArg) { omegaRad = omegaRadArg; }
    void setRotSpeedDeg(const float omegaDegArg) { setRotSpeedRad(degToRad(omegaDegArg)); }
    float getRotSpeedRad() const { return omegaRad; }
    float getRotSpeedDeg() const { return radToDeg(omegaRad); }
    void setColor(const Color& colorArg) { color = colorArg; }
    virtual void update() { move(); }
    void draw() const;
    void render();

    static float interp;
    ObjectState state{};
    ObjectState statep{};
    GLint glList{};
    PointsF verts;
    types::Bounds bounds;
    GeometryType geometryType{GeometryType::Point};
    int scoreReward{};

protected:
    float friction{};
    Color color;

private:
    virtual void onRender() const {} // Draws Object in its own coordinate system

    float omegaRad{};
    types::Vector v{};
    types::Vector a{};
    float fa{};
};
} // namespace game::objects

#endif // game_objects_Object
