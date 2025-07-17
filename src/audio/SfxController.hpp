#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockSfxController.hpp"
#if USE_ORIG(audio_SfxController)

#include <bass.h>
#include "audio/SfxSamples.hpp"

// #define NUM_BUFFERS 12

namespace audio
{
class SfxController
{
private:
    bool isPause{false};
    float volume{1.0f}; // range 0.0f - 1.0f
    SfxSamples samples;

protected:
    bool initSound();
    void freeSound();

public:
    bool open();
    void close();
    void setVolume(const float volumeNew);
    float getVolume() const { return volume; }
    void mute();
    void unmute();
    void stop();
    HSAMPLE getSample(int id) { return samples.getSample(static_cast<SampleId>(id)); }
};
} // namespace audio

#endif // audio_SfxController
