#include "Sound.hpp"

namespace audio
{

bool MusicEngineBass::open()
{
    return initSound();
}

void MusicEngineBass::close()
{
    freeSound();
}

bool MusicEngineBass::initSound()
{
    if (hMus) return true;
    hMus = BASS_StreamCreateFile(false, "sound/BWV1052.ogg", 0, 0, BASS_SAMPLE_LOOP);
    if (hMus) BASS_ChannelSetAttribute(hMus, BASS_ATTRIB_VOL, 0.4f);
    return true;
}

void MusicEngineBass::freeSound()
{
    if (hMus)
    {
        BASS_StreamFree(hMus);
        hMus = 0;
    }
}

void MusicEngineBass::setVolume(const float volumeNew)
{
    volume = volumeNew;
    BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, volume * 10000u);
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, volume * 10000u);
}

void MusicEngineBass::play()
{
    if (hMus)
    {
        channel = BASS_ChannelPlay(hMus, TRUE);
    }
}

void MusicEngineBass::pause()
{
    if (channel)
    {
        BASS_ChannelPause(hMus);
    }
}

void MusicEngineBass::stop()
{
    if (channel)
    {
        BASS_ChannelStop(hMus);
        channel = 0;
    }
}

void MusicEngineBass::mute()
{
    BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, 0);
}

void MusicEngineBass::unmute()
{
    BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, volume * 10000u);
}

void MusicEngineBass::slideVol(const float volumeNew, const DWORD time)
{
    if (BASS_ChannelIsActive(channel) == BASS_ACTIVE_STOPPED) return;
    BASS_ChannelSlideAttribute(channel, BASS_ATTRIB_VOL, volumeNew, time);
}

bool MusicEngineBass::isStarted() const
{
    DWORD r = BASS_ChannelIsActive(hMus);
    return r != BASS_ACTIVE_STOPPED;
}
} // namespace audio
