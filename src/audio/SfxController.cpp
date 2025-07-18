#include "SfxController.hpp"

#define MAX_CHANNEL_COUNT 5
#define NUM_SOURCES 12

namespace audio
{
bool SfxController::open()
{
    BASS_Init(-1, 44100, BASS_DEVICE_3D, NULL, NULL);
    samples.init();
    return true;
}

void SfxController::close()
{
    samples.free();
    BASS_Free();
}

void SfxController::stop()
{
    samples.stop();
    isPause = false;
}

void SfxController::setVolume(const float volumeNew)
{
    volume = volumeNew;
    BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, DWORD(volume * 10000));
}

void SfxController::mute()
{
    BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, 0);
}

void SfxController::unmute()
{
    BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, DWORD(volume * 10000));
}
} // namespace audio
