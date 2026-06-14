#pragma once

typedef struct MIX_Track MIX_Track;

class SfxSample
{
public:
    ~SfxSample();

    void init(const int sampleIdArg, const float volumeArg);
    void play();
    void pause();
    void stop();
    void setVolume(const float volumeArg);
    void slideVol(const float volumeArg, const int timeMs);

private:
    MIX_Track* loopTrack{}; // owned track for looping samples only; one-shots use the mixer pool
    int sampleId{-1};
    float volume{};
};
