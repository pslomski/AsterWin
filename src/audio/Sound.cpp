#include "Sound.hpp"
#include <stdlib.h>

namespace audio
{

MusicEngineBASS::MusicEngineBASS() : AudioController()
{
    m_Channel = 0;
    m_hMus = 0;
}

bool MusicEngineBASS::open()
{
    return initSound();
}

void MusicEngineBASS::close()
{
    freeSound();
}

bool MusicEngineBASS::initSound()
{
    if (m_hMus) return true;
    m_hMus = BASS_StreamCreateFile(false, "sound/BWV1052.ogg", 0, 0, BASS_SAMPLE_LOOP);
    if (m_hMus) BASS_ChannelSetAttribute(m_hMus, BASS_ATTRIB_VOL, 0.4f);
    return true;
}

void MusicEngineBASS::freeSound()
{
    if (m_hMus)
    {
        BASS_StreamFree(m_hMus);
        m_hMus = 0;
    }
}

void MusicEngineBASS::setVolume(const float volumeNew)
{
    volume = volumeNew;
    BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, DWORD(volume * 10000));
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, DWORD(volume * 10000));
}

void MusicEngineBASS::play(BOOL in_bRestart)
{
    if (m_hMus)
    {
        m_Channel = BASS_ChannelPlay(m_hMus, in_bRestart);
    }
}

void MusicEngineBASS::pause()
{
    if (m_Channel)
    {
        BASS_ChannelPause(m_hMus);
    }
}

void MusicEngineBASS::stop()
{
    if (m_Channel)
    {
        BASS_ChannelStop(m_hMus);
        m_Channel = 0;
    }
}

void MusicEngineBASS::mute()
{
    BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, 0);
}

void MusicEngineBASS::unmute()
{
    BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, DWORD(volume * 10000));
}

void MusicEngineBASS::slideVol(float in_NewVol, DWORD in_Time)
{
    if (BASS_ChannelIsActive(m_Channel) == BASS_ACTIVE_STOPPED) return;
    BASS_ChannelSlideAttribute(m_Channel, BASS_ATTRIB_VOL, in_NewVol, in_Time);
}

bool MusicEngineBASS::isStarted()
{
    DWORD r = BASS_ChannelIsActive(m_hMus);
    return r != BASS_ACTIVE_STOPPED;
}
} // namespace audio
