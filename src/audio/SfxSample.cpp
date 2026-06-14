#include "SfxSample.hpp"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <cassert>
#include "audio/AudioLib.hpp"
#include "audio/Sound.hpp"

SfxSample::~SfxSample()
{
    // SfxSample lives inside static-lifetime singletons, whose destructors run
    // after the mixer has been torn down at exit; only free the track while the
    // mixer is still alive.
    if (track && audio::audioLib.isInitialized())
    {
        MIX_DestroyTrack(track);
    }
}

void SfxSample::init(const int sampleIdArg, const float volumeArg)
{
    sampleId = sampleIdArg;
    volume = volumeArg;
}

void SfxSample::play()
{
    if (sampleId == -1) return;
    const audio::SampleRef ref = geSound.getSample(sampleId);
    if (!ref.audio) return;
    if (!track)
    {
        track = MIX_CreateTrack(audio::audioLib.mixer());
        if (!track) return;
    }
    MIX_SetTrackAudio(track, ref.audio);
    MIX_SetTrackGain(track, volume);
    if (ref.loop)
    {
        const SDL_PropertiesID props = SDL_CreateProperties();
        SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
        MIX_PlayTrack(track, props);
        SDL_DestroyProperties(props);
    }
    else
    {
        MIX_PlayTrack(track, 0);
    }
}

void SfxSample::pause()
{
    if (track) MIX_PauseTrack(track);
}

void SfxSample::stop()
{
    if (track) MIX_StopTrack(track, 0);
}

void SfxSample::setVolume(const float volumeArg)
{
    volume = volumeArg;
    if (track) MIX_SetTrackGain(track, volume);
}

void SfxSample::slideVol(const float volumeArg, const int timeMs)
{
    if (!track) return;
    if (volumeArg <= 0.0f)
    {
        MIX_StopTrack(track, MIX_TrackMSToFrames(track, timeMs));
    }
    else
    {
        MIX_SetTrackGain(track, volumeArg);
    }
}
