#pragma once

#include <gl/gl.h>
#include <math.h>
#include <windows.h>
#include "game/objects/TempObject.hpp"

class Bullet : public TempObject
{
public:
    Bullet();
    ~Bullet() {}

protected:
    void onRender() override;
};

typedef std::list<Bullet*> TvecBullet;
typedef std::list<Bullet*>::iterator TvecBulletIt;

class TShipDebris : public TempObject
{
private:
    Float m_Ratio;

protected:
    void onRender() override;

public:
    TShipDebris();
    ~TShipDebris() {}
    void update() override;
};

class AsterDebris : public TempObject
{
private:
    Float m_Ratio;

protected:
    void onRender() override;

public:
    AsterDebris();
    ~AsterDebris() {}
    void update() override;
};

class StarBlink : public TempObject
{
private:
    Float m_Ratio;
    Float m_Size;
    void Init();

protected:
    void onRender() override;

public:
    StarBlink();
    ~StarBlink() {}
    void update() override;
};
