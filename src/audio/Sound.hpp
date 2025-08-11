#pragma once

#include <bass.h>
#include "SfxController.hpp"

namespace audio
{
class MusicEngineBass
{
public:
    bool open();
    void close();
    void setVolume(const float volumeNew);
    float getVolume() const { return volume; }
    void mute();
    void unmute();
    void play();
    void pause();
    void stop();
    void slideVol(const float volumeNew, const DWORD time);
    bool isStarted() const;

private:
    bool initSound();
    void freeSound();

    bool isPause{false};
    float volume{1.0f}; // range 0.0f - 1.0f
    HSTREAM hMus{};
    HCHANNEL channel{};
};
} // namespace audio

inline audio::MusicEngineBass geMusic;
inline audio::SfxController geSound;
