#pragma once

#include "common/testing/mocks.hpp"
#if not USE_ORIG(audio_AudioLib)

#include <bass.h>
#include <gmock/gmock.h>
#include "audio/SampleFlags.hpp"

namespace audio
{
class AudioLib
{
public:
    MOCK_METHOD(void, init, ());
    MOCK_METHOD(void, free, ());
    MOCK_METHOD(HSAMPLE, loadSample, (const char*, const int, const SampleFlags));
    MOCK_METHOD(void, sampleStop, (const HSAMPLE));
    MOCK_METHOD(void, sampleFree, (const HSAMPLE));
    MOCK_METHOD(void, setVolume, (const float));
};

inline AudioLib audioLib;
} // namespace audio

#endif // audio_AudioLib
