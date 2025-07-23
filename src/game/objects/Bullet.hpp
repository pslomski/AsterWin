#pragma once

#include <gl/gl.h>
#include <math.h>
#include <windows.h>
#include "game/objects/TempObject.hpp"

namespace game::objects
{
class Bullet : public TempObject
{
public:
    Bullet();
    ~Bullet() {}

private:
    void onRender() override;
};

typedef std::list<Bullet*> TvecBullet;
typedef std::list<Bullet*>::iterator TvecBulletIt;

class AsterShards : public TempObject
{
public:
    AsterShards();
    ~AsterShards() {}

    void update() override;

private:
    void onRender() override;

    Float m_Ratio;
};

class StarBlink : public TempObject
{
public:
    StarBlink();
    ~StarBlink() {}
    void update() override;

private:
    void onRender() override;

    Float m_Ratio;
    Float m_Size;
    void Init();
};
} // namespace game::objects
