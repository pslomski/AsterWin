#pragma once

#include <bass.h>
#include <cstdint>
#include <map>
#include "audio/SampleId.hpp"

namespace audio
{
using Flags = uint32_t;

class SfxSamples
{
public:
    ~SfxSamples();

    void init();
    void free();
    void stop();
    HSAMPLE getSample(const SampleId id) { return samples[id]; }

private:
    void addSample(const SampleId sampleId, const char* name, const Flags flags = 0);

    using Samples = std::map<SampleId, HSAMPLE>;
    Samples samples;
};
} // namespace audio
