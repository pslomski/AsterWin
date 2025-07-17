#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockAudioController.hpp"
#if USE_ORIG(audio_AudioController)

namespace audio
{
class AudioController
{
public:
    virtual ~AudioController() { close(); }
    virtual bool open() = 0;
    virtual void close() {}
    virtual void setVolume(const float volumeNew) { volume = volumeNew; }
    virtual float getVolume() const { return volume; }
    virtual void mute() = 0;
    virtual void unmute() = 0;
    virtual void play() {};
    virtual void pause() {};
    virtual void stop() {};

protected:
    bool isPause{false};
    float volume{1.0f}; // range 0.0f - 1.0f
    virtual bool initSound() = 0;
    virtual void freeSound() = 0;
};
} // namespace audio

#endif // audio_AudioController
