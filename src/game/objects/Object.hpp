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
#include "game/types/Position.hpp"
#include "game/types/Types.hpp"
#include "game/types/Vector.hpp"

namespace game::objects
{
class Object
{
public:
    virtual ~Object();

    Float distance(const Object* object) const;
    void move();
    BoxF transform(const BoxF& seg) const;
    bool checkCollision(Object* pObiekt);
    void setXY(const Float x, const Float y)
    {
        pos.x = x;
        pos.y = y;
        posp = pos;
    }
    void setXY(const PointF& pt)
    {
        pos = pt;
        posp = pt;
    }
    void setV(const Float avx, const Float avy)
    {
        v.x = avx;
        v.y = avy;
    }
    void setV(const Float av);
    void setVA(const Float av, const Float alfa);
    void setRandV(const Float vmin, const Float vmax);
    Float getVX() const { return v.x; }
    Float getVY() const { return v.y; }
    Float getV() const;
    void setA(const Float aa);
    Float getA() const { return fa; }
    void setAlfa(const Float aalfa)
    {
        angle = correctAlfa(aalfa);
        alphap = angle;
    }
    Float getAlfa() const { return angle; }
    Float getRotSpeed() const { return omega; }
    void setRotSpeed(const Float omegaArg) { omega = omegaArg; }
    void setColor(const Float aRed = 1, const Float aGreen = 1, const Float aBlue = 1)
    {
        color.red = aRed;
        color.green = aGreen;
        color.blue = aBlue;
    }
    void setColor(const Color& in_clr) { color = in_clr; }
    virtual void update() { move(); }
    void draw() const;
    void render();

    static double interp;

    types::Position pos{}; // current pos
    types::Position posp{}; // previous pos
    GLint glList{};
    PointsF verts;
    GeometryType geometryType{GeometryType::Point};
    Float alphap;
    int scoreReward{};

protected:
    Float correctAlfa(Float alfa);
    void calcBounds(const PointsF& points);

    Float friction{};
    Color color;
    types::Bounds bounds;

private:
    virtual void onRender() const {} // Draws Object in its own coordinate system

    Float angle{};
    Float omega{};
    types::Vector v{};
    types::Vector a{};
    Float fa{};
};
} // namespace game::objects

#endif // game_objects_Object
