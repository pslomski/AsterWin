#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockSfxController.hpp"
#if USE_ORIG(audio_SfxController)

#include <bass.h>
#include "audio/SfxSamples.hpp"

namespace audio
{
class SfxController
{
public:
    bool open();
    void close();
    void setVolume(const float volumeNew);
    float getVolume() const { return volume; }
    void mute();
    void unmute();
    void stop();
    HSAMPLE getSample(int id) { return samples.get(static_cast<SampleId>(id)); }

private:
    float volume{1.0f}; // range 0.0f - 1.0f
    SfxSamples samples;
};
} // namespace audio

#endif // audio_SfxController
