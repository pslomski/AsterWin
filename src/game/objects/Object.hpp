#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockObject.hpp"
#if USE_ORIG(game_objects_Object)

#include <gl/gl.h>
#include "game/objects/Object.fwd.hpp"
#include "game/types/Bounds.hpp"
#include "game/types/Box.hpp"
#include "game/types/Color.hpp"
#include "game/types/ObjectGeom.hpp"
#include "game/types/Point.hpp"
#include "game/types/Types.hpp"

namespace game::objects
{
class Object
{
public:
    Object();
    virtual ~Object();

    Float distance(const Object* object) const;
    void move();
    BoxF transform(const BoxF& seg) const;
    bool checkCollision(Object* pObiekt);
    void setXY(const Float x, const Float y)
    {
        fx = x;
        fy = y;
        xp = x;
        yp = y;
    }
    void setXY(const PointF& pt)
    {
        fx = pt.x;
        fy = pt.y;
        xp = pt.x;
        yp = pt.y;
    }
    Float getX() const { return fx; }
    Float getY() const { return fy; }
    PointF getXY() { return {fx, fy}; }
    void setV(Float avx, Float avy)
    {
        fvx = avx;
        fvy = avy;
    }
    void setV(Float av);
    void setVA(Float av, Float alfa);
    void setRandV(Float vmin, Float vmax);
    Float getVX() { return fvx; }
    Float getVY() { return fvy; }
    Float getV();
    void setA(Float aa);
    Float getA() { return fa; }
    void setAlfa(Float aalfa)
    {
        angle = correctAlfa(aalfa);
        alphap = angle;
    }
    Float getAlfa() const { return angle; }
    Float getRotSpeed() const { return omega; }
    void setRotSpeed(const Float omegaArg) { omega = omegaArg; }
    void setColor(Float aRed = 1, Float aGreen = 1, Float aBlue = 1)
    {
        color.red = aRed;
        color.green = aGreen;
        color.blue = aBlue;
    }
    void setColor(Color& in_clr) { color = in_clr; }
    virtual void update() { move(); }
    void draw();
    void render();

    static double interp;

    GLint glList;
    PointsF verts;
    GeometryType geometryType;
    Float xp, yp, alphap;
    int scoreReward{};

protected:
    Float correctAlfa(Float alfa);
    void calcBounds(const PointsF& points);

    Float KDec;
    Color color;
    Bounds bounds;

private:
    virtual void onRender() {} // Draws Object in its own coordinate system

    Float angle;
    Float omega;
    Float fx, fy;
    Float fvx, fvy;
    Float fv;
    Float fax, fay;
    Float fa;
};
} // namespace game::objects

#endif // game_objects_Object
