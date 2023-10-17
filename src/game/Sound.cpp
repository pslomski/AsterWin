#include "Sound.hpp"
#include <stdlib.h>
#include "World.hpp"

SoundEngineBASS geSound;
MusicEngineBASS geMusic;

#define MAX_CHANNEL_COUNT 5

SoundEngineBASS::SoundEngineBASS() : SoundEngineBase()
{
    m_bSamplesLoaded = false;
    for (int i = 0; i < NUM_BUFFERS; ++i)
        Sample[i] = 0;
}

bool SoundEngineBASS::Open()
{
    // Initialize the default output device with 3D support
    BASS_Init(-1, 44100, BASS_DEVICE_3D, NULL, NULL);
    return InitSound();
}

void SoundEngineBASS::Close()
{
    FreeSound();
    BASS_Free();
}

bool SoundEngineBASS::InitSound()
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

void SoundEngineBASS::FreeSound()
{
    if (!m_bSamplesLoaded) return;
    for (int i = 0; i < NUM_BUFFERS; ++i)
    {
        BASS_SampleFree(Sample[i]);
        Sample[i] = 0;
    }
    m_bSamplesLoaded = false;
}

void SoundEngineBASS::SoundTest()
{
    m_sndTest.Play();
}

void SoundEngineBASS::Play()
{
    HCHANNEL ch[MAX_CHANNEL_COUNT];
    DWORD iCount;
    for (int i = 0; i < NUM_SOURCES; ++i)
    {
        iCount = BASS_SampleGetChannels(Sample[i], ch);
        for (DWORD j = 0; j < iCount; ++j)
        {
            BASS_ChannelPlay(ch[j], !m_bPause);
        }
    }
    m_bPause = false;
}

void SoundEngineBASS::Pause()
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
    m_bPause = true;
}

void SoundEngineBASS::Stop()
{
    for (int i = 0; i < NUM_SOURCES; ++i)
        BASS_SampleStop(Sample[i]);
    m_bPause = false;
}

void SoundEngineBASS::SetVolume(float in_Vol)
{
    m_Volume = in_Vol;
    BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, DWORD(m_Volume * 10000));
}

void SoundEngineBASS::Mute()
{
    BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, 0);
}

void SoundEngineBASS::Unmute()
{
    BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, DWORD(m_Volume * 10000));
}

MusicEngineBASS::MusicEngineBASS() : SoundEngineBase()
{
    m_Channel = 0;
    m_hMus = 0;
}

bool MusicEngineBASS::Open()
{
    // Initialize the default output device with 3D support
    BASS_Init(-1, 44100, BASS_DEVICE_3D, NULL, NULL);
    return InitSound();
}

void MusicEngineBASS::Close()
{
    FreeSound();
    BASS_Free();
}

bool MusicEngineBASS::InitSound()
{
    if (m_hMus) return true;

    // m_hMus=BASS_MusicLoad(false, "sound/bwv1052a.it", 0, 0, BASS_SAMPLE_LOOP,
    // 0);
    m_hMus = BASS_StreamCreateFile(false, "sound/BWV1052.ogg", 0, 0, BASS_SAMPLE_LOOP);
    if (m_hMus) BASS_ChannelSetAttribute(m_hMus, BASS_ATTRIB_VOL, 0.4f);
    return true;
}

void MusicEngineBASS::FreeSound()
{
    if (!m_hMus) return;

    // BASS_MusicFree(m_hMus);
    BASS_StreamFree(m_hMus);

    m_hMus = 0;
}

void MusicEngineBASS::SetVolume(float in_Vol)
{
    m_Volume = in_Vol;
    BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, DWORD(m_Volume * 10000));
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, DWORD(m_Volume * 10000));
}

void MusicEngineBASS::Play(BOOL in_bRestart)
{
    if (!m_hMus) return;
    //	if(!m_Channel)
    m_Channel = BASS_ChannelPlay(m_hMus, in_bRestart);
}

void MusicEngineBASS::Pause()
{
    if (m_Channel) BASS_ChannelPause(m_hMus);
}

void MusicEngineBASS::Stop()
{
    if (m_Channel) BASS_ChannelStop(m_hMus);
    m_Channel = 0;
}

void MusicEngineBASS::Mute()
{
    BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, 0);
}

void MusicEngineBASS::Unmute()
{
    BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, DWORD(m_Volume * 10000));
}

void MusicEngineBASS::SlideVol(float in_NewVol, DWORD in_Time)
{
    if (BASS_ChannelIsActive(m_Channel) == BASS_ACTIVE_STOPPED) return;
    BASS_ChannelSlideAttribute(m_Channel, BASS_ATTRIB_VOL, in_NewVol, in_Time);
}

bool MusicEngineBASS::IsStarted()
{
    DWORD r = BASS_ChannelIsActive(m_hMus);
    return r != BASS_ACTIVE_STOPPED;
}

/////////////////////////////////////////////////////////////////////////////////////////////
