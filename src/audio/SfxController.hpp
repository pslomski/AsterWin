#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockSfxController.hpp"
#if USE_ORIG(audio_SfxController)

#include <bass.h>
#include "audio/ObjectSound.hpp"

#define NUM_BUFFERS 12

namespace audio
{
class SfxController
{
private:
    bool isPause{false};
    float volume{1.0f}; // range 0.0f - 1.0f
    HSAMPLE Sample[NUM_BUFFERS];
    bool m_bSamplesLoaded;
    ObjectSound m_sndTest; // setting sound volume in Options menu
protected:
    bool initSound();
    void freeSound();

public:
    SfxController();
    bool open();
    void close();
    void setVolume(const float volumeNew);
    float getVolume() const { return volume; }
    void mute();
    void unmute();
    void stop();
    void soundTest();
    HSAMPLE getSample(int id) { return Sample[id]; }
};
} // namespace audio

#endif // audio_SfxController
