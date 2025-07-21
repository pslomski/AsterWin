#pragma once

#include "bass.h"

class ObjSoundBASS
{
public:
    void Init(int in_ID, float in_Volume)
    {
        m_SoundID = in_ID;
        m_Volume = in_Volume;
    }
    void Play();
    void Pause();
    void Stop();
    void SetVolume(float in_Volume);
    void SetPitch(float in_Pitch) {}
    void SlideVol(float in_NewVol, DWORD in_Time);

private:
    int m_SoundID{-1};
    float m_Volume{};
    HCHANNEL m_Channel{};
};

using ObjectSound = ObjSoundBASS;
