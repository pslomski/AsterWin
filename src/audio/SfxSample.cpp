#include <assert.h>
#include "SfxSample.hpp"
#include "audio/Sound.hpp"


void SfxSample::Init(int in_ID, float in_Volume)
{
    m_SoundID = in_ID;
    m_Volume = in_Volume;
}

void SfxSample::Play()
{
    assert(m_SoundID != -1);
    if (m_SoundID == -1) return;
    m_Channel = BASS_SampleGetChannel(geSound.getSample(m_SoundID), FALSE);
    assert(m_Channel != 0);
    BASS_ChannelSetAttribute(m_Channel, BASS_ATTRIB_VOL, m_Volume);
    BOOL bRes = BASS_ChannelPlay(m_Channel, FALSE);
    assert(bRes == TRUE);
}

void SfxSample::Pause()
{
    assert(m_SoundID != -1);
    if (m_SoundID == -1) return;
    BASS_ChannelPause(m_Channel);
}

void SfxSample::Stop()
{
    assert(m_SoundID != -1);
    if (m_SoundID == -1) return;
    BASS_ChannelStop(m_Channel);
}

void SfxSample::SetVolume(float in_Volume)
{
    assert(m_SoundID != -1);
    m_Volume = in_Volume;
    if (m_SoundID == -1) return;
    if (BASS_ChannelIsActive(m_Channel) == BASS_ACTIVE_STOPPED)
        m_Channel = BASS_SampleGetChannel(geSound.getSample(m_SoundID), FALSE);
    BASS_ChannelSetAttribute(m_Channel, BASS_ATTRIB_VOL, m_Volume);
}

void SfxSample::SlideVol(float in_NewVol, DWORD in_Time)
{
    assert(m_SoundID != -1);
    if (m_SoundID == -1) return;
    if (BASS_ChannelIsActive(m_Channel) == BASS_ACTIVE_STOPPED) return;
    BASS_ChannelSlideAttribute(m_Channel, BASS_ATTRIB_VOL, in_NewVol, in_Time);
}
