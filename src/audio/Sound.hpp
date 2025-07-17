#pragma once

#include <bass.h>
#include "SfxController.hpp"
#include "audio/AudioController.hpp"
#include "audio/Consts.hpp"
#include "audio/ObjectSound.hpp"

namespace audio
{
class MusicEngineBASS : public AudioController
{
private:
    HSTREAM m_hMus;
    HCHANNEL m_Channel;

protected:
    bool initSound() override;
    void freeSound() override;

public:
    MusicEngineBASS();
    bool open() override;
    void close() override;
    void setVolume(const float volumeNew) override;
    void mute() override;
    void unmute() override;
    void play(BOOL in_bRestart = false);
    void pause() override;
    void stop() override;
    void slideVol(float in_NewVol, DWORD in_Time);
    bool isStarted();
};
} // namespace audio

inline audio::MusicEngineBASS geMusic;
inline audio::SfxController geSound;
