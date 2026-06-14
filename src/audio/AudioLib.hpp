#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockAudioLib.hpp"
#if USE_ORIG(audio_AudioLib)

#include <vector>

typedef struct MIX_Mixer MIX_Mixer;
typedef struct MIX_Audio MIX_Audio;
typedef struct MIX_Track MIX_Track;

namespace audio
{
class AudioLib
{
public:
    void init();
    void free();
    MIX_Audio* loadSample(const char* name);
    void freeSample(MIX_Audio* sample);
    // Play a sound once on a pooled track, independent of any caller's lifetime
    // (so a one-shot effect keeps playing after the object that fired it dies).
    void playOneShot(MIX_Audio* sample, float gain);
    void stopAll();
    void setVolume(const float volume);
    MIX_Mixer* mixer() const { return mixerHandle; }
    bool isInitialized() const { return mixerHandle != nullptr; }

private:
    MIX_Mixer* mixerHandle{};
    std::vector<MIX_Track*> oneShotPool;
};

inline AudioLib audioLib;
} // namespace audio

#endif // #if USE_ORIG(audio_AudioLib)
