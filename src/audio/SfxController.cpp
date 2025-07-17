#include "SfxController.hpp"
#include "audio/Consts.hpp"

#define MAX_CHANNEL_COUNT 5
#define NUM_SOURCES 12

namespace audio
{
SoundEngineBASS::SoundEngineBASS() : AudioController()
{
    m_bSamplesLoaded = false;
    for (int i = 0; i < NUM_BUFFERS; ++i)
    {
        Sample[i] = 0;
    }
}

bool SoundEngineBASS::open()
{
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

} // namespace audio
