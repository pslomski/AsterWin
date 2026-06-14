#pragma once

#include "common/testing/mocks.hpp"
#if not USE_ORIG(audio_AudioLib)

#include <gmock/gmock.h>

typedef struct MIX_Audio MIX_Audio;

namespace audio
{
class AudioLib
{
public:
    MOCK_METHOD(void, init, ());
    MOCK_METHOD(void, free, ());
    MOCK_METHOD(MIX_Audio*, loadSample, (const char*));
    MOCK_METHOD(void, freeSample, (MIX_Audio*));
    MOCK_METHOD(void, stopAll, ());
    MOCK_METHOD(void, setVolume, (const float));
};

inline AudioLib audioLib;
} // namespace audio

#endif // audio_AudioLib
