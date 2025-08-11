#pragma once

#include <bass.h>
#include "SfxController.hpp"

namespace audio
{
class MusicEngineBASS
{
private:
    HSTREAM m_hMus;
    HCHANNEL m_Channel;

protected:
    bool initSound();
    void freeSound();

    bool isPause{false};
    float volume{1.0f}; // range 0.0f - 1.0f

public:
    MusicEngineBASS();
    bool open();
    void close();
    void setVolume(const float volumeNew);
    float getVolume() const { return volume; }
    void mute();
    void unmute();
    void play();
    void pause();
    void stop();
    void slideVol(float in_NewVol, DWORD in_Time);
    bool isStarted();
};
} // namespace audio

inline audio::MusicEngineBASS geMusic;
inline audio::SfxController geSound;
