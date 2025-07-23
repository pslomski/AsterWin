#include "SfxSample.hpp"
#include <assert.h>
#include "audio/Sound.hpp"

void SfxSample::init(const int sampleIdArg, const float volumeArg)
{
    sampleId = sampleIdArg;
    volume = volumeArg;
}

void SfxSample::play()
{
    assert(sampleId != -1);
    if (sampleId == -1) return;
    channel = BASS_SampleGetChannel(geSound.getSample(sampleId), 0);
    assert(channel != 0);
    BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, volume);
    BOOL bRes = BASS_ChannelPlay(channel, FALSE);
    assert(bRes == TRUE);
}

void SfxSample::pause()
{
    assert(sampleId != -1);
    if (sampleId == -1) return;
    BASS_ChannelPause(channel);
}

void SfxSample::stop()
{
    assert(sampleId != -1);
    if (sampleId == -1) return;
    BASS_ChannelStop(channel);
}

void SfxSample::setVolume(const float volumeArg)
{
    assert(sampleId != -1);
    volume = volumeArg;
    if (sampleId == -1) return;
    if (BASS_ChannelIsActive(channel) == BASS_ACTIVE_STOPPED)
        channel = BASS_SampleGetChannel(geSound.getSample(sampleId), FALSE);
    BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, volume);
}

void SfxSample::slideVol(const float volumeArg, const DWORD time)
{
    assert(sampleId != -1);
    if (sampleId == -1) return;
    if (BASS_ChannelIsActive(channel) == BASS_ACTIVE_STOPPED) return;
    BASS_ChannelSlideAttribute(channel, BASS_ATTRIB_VOL, volumeArg, time);
}
