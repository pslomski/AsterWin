#pragma once

#include "audio/AudioController.hpp"
#include "audio/Consts.hpp"
#include "audio/ObjectSound.hpp"
#include "bass.h"

#define NUM_BUFFERS 12
#define NUM_SOURCES 12

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
inline audio::SoundEngineBASS geSound;
