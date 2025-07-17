#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockAudioController.hpp"
#if USE_ORIG(audio_AudioController)

namespace audio
{
class AudioController
{
public:
    virtual ~AudioController() { Close(); }
    virtual bool Open() = 0;
    virtual void Close() {}
    virtual void SetVolume(float in_Vol) { m_Volume = in_Vol; }
    virtual float GetVolume() { return m_Volume; }
    virtual void Mute() = 0;
    virtual void Unmute() = 0;
    virtual void Play() {};
    virtual void Pause() {};
    virtual void Stop() {};

protected:
    bool m_bPause{false};
    float m_Volume{1.0f}; // range 0.0f - 1.0f
    virtual bool InitSound() = 0;
    virtual void FreeSound() = 0;
};
} // namespace audio

#endif // audio_AudioController
