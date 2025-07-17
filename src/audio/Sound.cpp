#include "Sound.hpp"
#include <stdlib.h>

namespace audio
{
SoundEngineBASS geSound;
MusicEngineBASS geMusic;

#define MAX_CHANNEL_COUNT 5

SoundEngineBASS::SoundEngineBASS() : AudioController()
{
    m_bSamplesLoaded = false;
    for (int i = 0; i < NUM_BUFFERS; ++i)
        Sample[i] = 0;
}

bool SoundEngineBASS::open()
{
    // Initialize the default output device with 3D support
    BASS_Init(-1, 44100, BASS_DEVICE_3D, NULL, NULL);
    return initSound();
}

void SoundEngineBASS::close()
{
    freeSound();
    BASS_Free();
}

bool SoundEngineBASS::initSound()
{
    if (m_bSamplesLoaded) return true;

    Sample[SND_SHIP_ENGINE] =
        BASS_SampleLoad(FALSE, "sound/ShipEngine.ogg", 0, 0, 1, BASS_SAMPLE_OVER_VOL | BASS_SAMPLE_LOOP);
    Sample[SND_SHIP_FIRE] = BASS_SampleLoad(FALSE, "sound/ShipFire.ogg", 0, 0, 1, BASS_SAMPLE_OVER_VOL);
    Sample[SND_SHIP_CRASH] = BASS_SampleLoad(FALSE, "sound/ShipCrash.ogg", 0, 0, 1, BASS_SAMPLE_OVER_VOL);
    Sample[SND_ASTER_CRASH1] =
        BASS_SampleLoad(FALSE, "sound/AsterCrash1.ogg", 0, 0, MAX_CHANNEL_COUNT, BASS_SAMPLE_OVER_VOL);
    Sample[SND_ASTER_CRASH2] =
        BASS_SampleLoad(FALSE, "sound/AsterCrash2.ogg", 0, 0, MAX_CHANNEL_COUNT, BASS_SAMPLE_OVER_VOL);
    Sample[SND_ASTER_CRASH3] =
        BASS_SampleLoad(FALSE, "sound/AsterCrash3.ogg", 0, 0, MAX_CHANNEL_COUNT, BASS_SAMPLE_OVER_VOL);
    Sample[SND_UFO_ENGINE] =
        BASS_SampleLoad(FALSE, "sound/UfoEngine.ogg", 0, 0, 1, BASS_SAMPLE_OVER_VOL | BASS_SAMPLE_LOOP);
    Sample[SND_START_BEEP] = BASS_SampleLoad(FALSE, "sound/StartBeep.ogg", 0, 0, 1, BASS_SAMPLE_OVER_VOL);
    Sample[SND_BONUS_BEEP] =
        BASS_SampleLoad(FALSE, "sound/BonusBeep.ogg", 0, 0, MAX_CHANNEL_COUNT, BASS_SAMPLE_OVER_VOL);
    Sample[SND_BROOM] = BASS_SampleLoad(FALSE, "sound/Broom.ogg", 0, 0, 1, BASS_SAMPLE_OVER_VOL);
    Sample[SND_POWERUP] = BASS_SampleLoad(FALSE, "sound/PowerUp.ogg", 0, 0, 1, BASS_SAMPLE_OVER_VOL);
    Sample[SND_SHIP_FIRE_POWER] = BASS_SampleLoad(FALSE, "sound/ShipFirePower.ogg", 0, 0, 1, BASS_SAMPLE_OVER_VOL);

    m_sndTest.Init(SND_SHIP_FIRE, SND_VOL_SHIP_FIRE);
    m_bSamplesLoaded = true;
    return true;
}

void SoundEngineBASS::freeSound()
{
    if (!m_bSamplesLoaded) return;
    for (int i = 0; i < NUM_BUFFERS; ++i)
    {
        BASS_SampleFree(Sample[i]);
        Sample[i] = 0;
    }
    m_bSamplesLoaded = false;
}

void SoundEngineBASS::soundTest()
{
    m_sndTest.Play();
}

void SoundEngineBASS::play()
{
    HCHANNEL ch[MAX_CHANNEL_COUNT];
    DWORD iCount;
    for (int i = 0; i < NUM_SOURCES; ++i)
    {
        iCount = BASS_SampleGetChannels(Sample[i], ch);
        for (DWORD j = 0; j < iCount; ++j)
        {
            BASS_ChannelPlay(ch[j], not isPause);
        }
    }
    isPause = false;
}

void SoundEngineBASS::pause()
{
    HCHANNEL ch[MAX_CHANNEL_COUNT];
    DWORD iCount;
    for (int i = 0; i < NUM_SOURCES; ++i)
    {
        iCount = BASS_SampleGetChannels(Sample[i], ch);
        for (DWORD j = 0; j < iCount; ++j)
        {
            BASS_ChannelPause(ch[j]);
        }
    }
    isPause = true;
}

void SoundEngineBASS::stop()
{
    for (int i = 0; i < NUM_SOURCES; ++i)
        BASS_SampleStop(Sample[i]);
    isPause = false;
}

void SoundEngineBASS::setVolume(const float volumeNew)
{
    volume = volumeNew;
    BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, DWORD(volume * 10000));
}

void SoundEngineBASS::mute()
{
    BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, 0);
}

void SoundEngineBASS::unmute()
{
    BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, DWORD(volume * 10000));
}

MusicEngineBASS::MusicEngineBASS() : AudioController()
{
    m_Channel = 0;
    m_hMus = 0;
}

bool MusicEngineBASS::open()
{
    BASS_Init(-1, 44100, BASS_DEVICE_3D, NULL, NULL);
    return initSound();
}

void MusicEngineBASS::close()
{
    freeSound();
    BASS_Free();
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
