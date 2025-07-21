#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockSfxSamples.hpp"
#if USE_ORIG(audio_SfxSamples)

#include <bass.h>
#include <map>
#include "audio/SampleFlags.hpp"
#include "audio/SampleId.hpp"

namespace audio
{
class SfxSamples
{
public:
    ~SfxSamples();

    void init();
    void free();
    void stop();
    HSAMPLE get(const SampleId id) { return samples[id]; }

private:
    void add(const SampleId sampleId, const char* name, const SampleFlags flags = 0);

    std::map<SampleId, HSAMPLE> samples;
};
} // namespace audio

#endif // audio_SfxSamples
