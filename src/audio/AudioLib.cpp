#include "AudioLib.hpp"

namespace
{
DWORD getBassFlags(const audio::SampleFlags flags)
{
    DWORD bassFlags{BASS_SAMPLE_OVER_POS};
    if (flags & audio::FLAG_SAMPLE_LOOP)
    {
        bassFlags |= BASS_SAMPLE_LOOP;
    }
    if (flags & audio::FLAG_SAMPLE_OVER_VOL)
    {
        bassFlags |= BASS_SAMPLE_OVER_VOL;
    }
    return bassFlags;
}
} // namespace

namespace audio
{
void AudioLib::init()
{
    BASS_Init(-1, 44100, BASS_DEVICE_3D, nullptr, nullptr);
}

void AudioLib::free()
{
    BASS_Free();
}

HSAMPLE AudioLib::loadSample(const char* name, const int maxSampleCount, const SampleFlags flags)
{
    return BASS_SampleLoad(FALSE, name, 0, 0, maxSampleCount, getBassFlags(flags));
}

void AudioLib::sampleStop(const HSAMPLE sample)
{
    if (sample != 0)
    {
        BASS_SampleStop(sample);
    }
}

void AudioLib::sampleFree(const HSAMPLE sample)
{
    if (sample != 0)
    {
        BASS_SampleFree(sample);
    }
}

void AudioLib::setVolume(const float volume)
{
    BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, DWORD(volume * 10000));
}
} // namespace audio
