#include "SfxController.hpp"
#include "audio/AudioLib.hpp"

namespace audio
{
bool SfxController::open()
{
    samples.init();
    return true;
}

void SfxController::close()
{
    samples.free();
}

void SfxController::stop()
{
    samples.stop();
    isPause = false;
}

void SfxController::setVolume(const float volumeNew)
{
    volume = volumeNew;
    audioLib.setVolume(volume);
}

void SfxController::mute()
{
    audioLib.setVolume(0);
}

void SfxController::unmute()
{
    audioLib.setVolume(volume);
}
} // namespace audio
