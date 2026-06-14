#include "AudioLib.hpp"
#include <SDL3_mixer/SDL_mixer.h>

namespace audio
{
void AudioLib::init()
{
    if (mixerHandle) return;
    MIX_Init();
    mixerHandle = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
}

void AudioLib::free()
{
    if (mixerHandle)
    {
        MIX_DestroyMixer(mixerHandle);
        mixerHandle = nullptr;
    }
    MIX_Quit();
}

MIX_Audio* AudioLib::loadSample(const char* name)
{
    if (!mixerHandle) return nullptr;
    return MIX_LoadAudio(mixerHandle, name, true /*predecode*/);
}

void AudioLib::freeSample(MIX_Audio* sample)
{
    if (sample) MIX_DestroyAudio(sample);
}

void AudioLib::stopAll()
{
    if (mixerHandle) MIX_StopAllTracks(mixerHandle, 0);
}

void AudioLib::setVolume(const float volume)
{
    if (mixerHandle) MIX_SetMixerGain(mixerHandle, volume);
}
} // namespace audio
