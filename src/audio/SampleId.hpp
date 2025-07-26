#pragma once

#include "audio/Consts.hpp"

namespace audio
{
enum class SampleId
{
    shipEngine = SND_SHIP_ENGINE,
    shipFire = SND_SHIP_FIRE,
    shipCrash = SND_SHIP_CRASH,
    asterCrash1 = SND_ASTER_CRASH1,
    asterCrash2 = SND_ASTER_CRASH2,
    asterCrash3 = SND_ASTER_CRASH3,
    ufoEngine = SND_UFO_ENGINE,
    startBeep = SND_START_BEEP,
    bonusBeep = SND_BONUS_BEEP,
    broom = SND_BROOM,
    powerUp = SND_POWERUP,
    shipFirePower = SND_SHIP_FIRE_POWER
};
} // namespace audio
