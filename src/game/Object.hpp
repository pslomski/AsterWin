#pragma once

#include <gl/gl.h>
#include <list>
#include <math.h>
#include <windows.h>
#include "types/Bounds.hpp"
#include "types/Box.hpp"
#include "types/Color.hpp"
#include "types/ObjectGeom.hpp"
#include "types/Point.hpp"
#include "types/Types.hpp"
#include "utils/TimeInterval.hpp"
#include "utils/Tools.hpp" //TODO: Remove dependency to Tools.hpp in Object.hpp

class Object
{
public:
    Object();
    virtual ~Object()
    {
        if (glList) glDeleteLists(glList, 1);
    }

    Float distance(const Object* object) const;
    void move();
    BoxF transform(const BoxF& seg);
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
    Float getV() { return sqrt(fvx * fvx + fvy * fvy); }
    void setA(Float aa);
    Float getA() { return fa; }
    void setAlfa(Float aalfa)
    {
        angle = correctAlfa(aalfa);
        alphap = angle;
    }
    Float getAlfa() { return angle; }
    Float getRotSpeed() { return omega; }
    void setRotSpeed(Float omega) { omega = omega; }
    void setColor(Float aRed = 1, Float aGreen = 1, Float aBlue = 1)
    {
        color.red = aRed;
        color.green = aGreen;
        color.blue = aBlue;
    }
    void setColor(Color& in_clr) { color = in_clr; }
    virtual void update() { move(); }
    virtual void draw();
    virtual bool expired() { return false; }
    void render();

    static Float dt;
    GLint glList;
    PointsF verts;
    GeometryType geometryType;
    Float xp, yp, alphap;
    int scoreReward{};

protected:
    virtual void OnRender() {} // Draws Object in its own coordinate system
    Float correctAlfa(Float alfa);
    void calcBounds(const PointsF& points);

    Float KDec;
    Color color;
    Bounds bounds;

private:
    Float angle;
    Float omega;
    Float fx, fy;
    Float fvx, fvy;
    Float fv;
    Float fax, fay;
    Float fa;
};

typedef std::list<Object*> ObjectList;
typedef std::list<Object*>::iterator TvecObiektIt;

class TempObject : public Object
{
public:
    TempObject();
    explicit TempObject(const Float lifeTime);
    bool expired() override;
    void setExpired();

    utils::TimeInterval lifeTime;
};
