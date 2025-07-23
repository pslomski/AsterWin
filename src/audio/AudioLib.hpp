#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockAudioLib.hpp"
#if USE_ORIG(audio_AudioLib)

#include <audio/SampleFlags.hpp>
#include <bass.h>

namespace audio
{
class AudioLib
{
public:
    void init();
    void free();
    HSAMPLE loadSample(const char* name, const int maxSampleCount, const SampleFlags flags);
    void sampleStop(const HSAMPLE sample);
    void sampleFree(const HSAMPLE sample);
    void setVolume(const float volume);
};

inline AudioLib audioLib;
} // namespace audio

#endif // #if USE_ORIG(audio_AudioLib)
