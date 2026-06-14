#pragma once

#include "common/testing/mocks.hpp"
#include "ut/mocks/MockSfxSamples.hpp"
#if USE_ORIG(audio_SfxSamples)

#include <map>
#include "audio/SampleFlags.hpp"
#include "audio/SampleId.hpp"

typedef struct MIX_Audio MIX_Audio;

namespace audio
{
struct SampleRef
{
    MIX_Audio* audio{};
    bool loop{};
};

class SfxSamples
{
public:
    ~SfxSamples();

    void init();
    void free();
    SampleRef get(const SampleId id) { return samples[id]; }

private:
    void add(const SampleId sampleId, const char* name, const SampleFlags flags = 0);

    std::map<SampleId, SampleRef> samples;
};
} // namespace audio

#endif // audio_SfxSamples
