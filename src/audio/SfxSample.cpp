#include "SfxSample.hpp"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "audio/AudioLib.hpp"
#include "audio/Sound.hpp"

SfxSample::~SfxSample()
{
    // SfxSample lives inside static-lifetime singletons / game objects, whose
    // destructors can run after the mixer is torn down at exit; only free the
    // track while the mixer is still alive.
    if (loopTrack && audio::audioLib.isInitialized())
    {
        MIX_DestroyTrack(loopTrack);
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
    if (!ref.loop)
    {
        // Fire-and-forget: must outlive this object (e.g. an asteroid is
        // deleted the instant its explosion sound starts).
        audio::audioLib.playOneShot(ref.audio, volume);
        return;
    }
    if (!loopTrack)
    {
        loopTrack = MIX_CreateTrack(audio::audioLib.mixer());
        if (!loopTrack) return;
    }
    MIX_SetTrackAudio(loopTrack, ref.audio);
    MIX_SetTrackGain(loopTrack, volume);
    const SDL_PropertiesID props = SDL_CreateProperties();
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    MIX_PlayTrack(loopTrack, props);
    SDL_DestroyProperties(props);
}

void SfxSample::pause()
{
    if (loopTrack) MIX_PauseTrack(loopTrack);
}

void SfxSample::stop()
{
    if (loopTrack) MIX_StopTrack(loopTrack, 0);
}

void SfxSample::setVolume(const float volumeArg)
{
    volume = volumeArg;
    if (loopTrack) MIX_SetTrackGain(loopTrack, volume);
}

void SfxSample::slideVol(const float volumeArg, const int timeMs)
{
    if (!loopTrack) return;
    if (volumeArg <= 0.0f)
    {
        MIX_StopTrack(loopTrack, MIX_TrackMSToFrames(loopTrack, timeMs));
    }
    else
    {
        MIX_SetTrackGain(loopTrack, volumeArg);
    }
}
