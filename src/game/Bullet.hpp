#pragma once

#include <gl\gl.h> //TODO Change to gl/gl.h
#include <math.h>
#include <windows.h>
#include "Object.hpp"

class Bullet : public TempObject
{
public:
    Bullet();
    ~Bullet() {}

protected:
    void OnRender() override;
};

typedef std::list<Bullet*> TvecBullet;
typedef std::list<Bullet*>::iterator TvecBulletIt;

class TShipDebris : public TempObject
{
private:
    Float m_Ratio;

protected:
    void OnRender() override;

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
    void OnRender() override;

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
    void OnRender() override;

public:
    StarBlink();
    ~StarBlink() {}
    void update() override;
};
