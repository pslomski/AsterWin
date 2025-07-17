#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockSfxController.hpp"
#if USE_ORIG(audio_SfxController)

#include <bass.h>
#include "audio/AudioController.hpp"
#include "audio/ObjectSound.hpp"

#define NUM_BUFFERS 12

namespace audio
{
class SoundEngineBASS : public AudioController
{
private:
    HSAMPLE Sample[NUM_BUFFERS];
    bool m_bSamplesLoaded;
    ObjectSound m_sndTest; // setting sound volume in Options menu
protected:
    bool initSound() override;
    void freeSound() override;

public:
    SoundEngineBASS();
    bool open() override;
    void close() override;
    void setVolume(const float volumeNew) override;
    void mute() override;
    void unmute() override;
    void play() override;
    void pause() override;
    void stop() override;
    void soundTest();
    HSAMPLE getSample(int id) { return Sample[id]; }
};
} // namespace audio

#endif // audio_SfxController
