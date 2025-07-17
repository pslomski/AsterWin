#pragma once

#include "audio/ObjectSound.hpp"
#include "bass.h"

#define NUM_BUFFERS 12
#define NUM_SOURCES 12

#define SND_SHIP_ENGINE 0
#define SND_SHIP_FIRE 1
#define SND_SHIP_CRASH 2
#define SND_ASTER_CRASH1 3
#define SND_ASTER_CRASH2 4
#define SND_ASTER_CRASH3 5
#define SND_UFO_ENGINE 6
#define SND_START_BEEP 7
#define SND_BONUS_BEEP 8
#define SND_BROOM 9
#define SND_POWERUP 10
#define SND_SHIP_FIRE_POWER 11

#define SND_VOL_SHIP_ENGINE 0.7f
#define SND_VOL_SHIP_FIRE 0.5f
#define SND_VOL_SHIP_CRASH 1.0f
#define SND_VOL_ASTER_CRASH1 1.0f
#define SND_VOL_ASTER_CRASH2 0.9f
#define SND_VOL_ASTER_CRASH3 1.0f
#define SND_VOL_UFO_ENGINE 0.4f
#define SND_VOL_START_BEEP 0.7f
#define SND_VOL_BONUS_BEEP 0.4f
#define SND_VOL_BROOM 0.4f
#define SND_VOL_POWERUP 0.5f
#define SND_VOL_SHIP_FIRE_POWER 0.4f

class SoundEngineBase
{
public:
    SoundEngineBase()
    {
        m_Volume = 1.0;
        m_bPause = false;
    }
    virtual ~SoundEngineBase() { Close(); }
    virtual bool Open() = 0;
    virtual void Close() {}
    virtual void SetVolume(float in_Vol) { m_Volume = in_Vol; } // ustawia glosnosc dzwiekow w skali 0-1.
    virtual float GetVolume() { return m_Volume; } // gloscnosc dzwiekow w skali 0-1. 0 oznacza wylaczona
    virtual void Mute() = 0;
    virtual void Unmute() = 0;
    virtual void Play() {};
    virtual void Pause() {};
    virtual void Stop() {};

protected:
    bool m_bPause;
    float m_Volume;
    virtual bool InitSound() = 0;
    virtual void FreeSound() = 0;
};

class SoundEngineBASS : public SoundEngineBase
{
private:
    HSAMPLE Sample[NUM_BUFFERS];
    bool m_bSamplesLoaded;
    ObjectSound m_sndTest; // setting sound volume in Options menu
protected:
    bool InitSound();
    void FreeSound();

public:
    SoundEngineBASS();
    bool Open();
    void Close();
    void SetVolume(float in_Vol);
    void Mute();
    void Unmute();
    void Play();
    void Pause();
    void Stop();
    void SoundTest();
    HSAMPLE GetSample(int ID) { return Sample[ID]; }
};

class MusicEngineBASS : public SoundEngineBase
{
private:
    HSTREAM m_hMus;
    HCHANNEL m_Channel;

protected:
    bool InitSound();
    void FreeSound();

public:
    MusicEngineBASS();
    bool Open();
    void Close();
    void SetVolume(float in_Vol);
    void Mute();
    void Unmute();
    void Play(BOOL in_bRestart = false);
    void Pause();
    void Stop();
    void SlideVol(float in_NewVol, DWORD in_Time);
    bool IsStarted();
};

extern MusicEngineBASS geMusic;
extern SoundEngineBASS geSound;
