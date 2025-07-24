#pragma once

#include "bass.h"

class SfxSample
{
public:
    void init(const int sampleIdArg, const float volumeArg);
    void play();
    void pause();
    void stop();
    void setVolume(const float volumeArg);
    void setPitch([[maybe_unused]] const float pitch) {} // TODO: Implement pitch control if possible
    void slideVol(const float volumeArg, const DWORD time);

private:
    int sampleId{-1};
    float volume{};
    HCHANNEL channel{};
};
