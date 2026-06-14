#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockAudioLib.hpp"
#if USE_ORIG(audio_AudioLib)

typedef struct MIX_Mixer MIX_Mixer;
typedef struct MIX_Audio MIX_Audio;

namespace audio
{
class AudioLib
{
public:
    void init();
    void free();
    MIX_Audio* loadSample(const char* name);
    void freeSample(MIX_Audio* sample);
    void stopAll();
    void setVolume(const float volume);
    MIX_Mixer* mixer() const { return mixerHandle; }
    bool isInitialized() const { return mixerHandle != nullptr; }

private:
    MIX_Mixer* mixerHandle{};
};

inline AudioLib audioLib;
} // namespace audio

#endif // #if USE_ORIG(audio_AudioLib)
