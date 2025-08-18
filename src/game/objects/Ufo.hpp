#pragma once

#include "Bullet.hpp"
#include "audio/SfxSample.hpp"
#include "game/objects/Bullets.hpp"
#include "game/objects/Object.hpp"
#include "game/objects/TempObjects.hpp"

namespace game::objects
{
class Ufo : public Object
{
public:
    Ufo();
    ~Ufo();

    void update() override;
    void action(Bullets& bullets);
    Bullet* fireBullet(const PointF& pt);
    void crash(TempObjects& vecObiekty);

    Object* pShip{nullptr};
    Object* pAster{nullptr};
    SfxSample sndEngine;
    SfxSample sndCrash;

private:
    void onRender() const override;

    float CheckTime{0.7f};
    float CheckTimeElapsed{};
    float MoveTime{3.0f};
    float MoveTimeElapsed{};
    float FireTime{2.0f};
    float FireTimeElapsed{};
};
} // namespace game::objects
