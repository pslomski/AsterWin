#include "SfxSamples.hpp"
#include <sstream>
#include "audio/AudioLib.hpp"
#include "audio/SampleFlags.hpp"

namespace audio
{
SfxSamples::~SfxSamples()
{
    free();
}

void SfxSamples::stop()
{
    for (const auto& sample : samples)
    {
        audioLib.sampleStop(sample.second);
    }
}

void SfxSamples::init()
{
    if (not samples.empty())
    {
        return; // Already initialized
    }
    constexpr auto maxSampleCount{5};
    add(SampleId::shipEngine, "ShipEngine", 1, FLAG_SAMPLE_LOOP);
    add(SampleId::shipFire, "ShipFire", 1);
    add(SampleId::shipCrash, "ShipCrash", 1);
    add(SampleId::asterCrash1, "AsterCrash1", maxSampleCount);
    add(SampleId::asterCrash2, "AsterCrash2", maxSampleCount);
    add(SampleId::asterCrash3, "AsterCrash3", maxSampleCount);
    add(SampleId::ufoEngine, "UfoEngine", 1, FLAG_SAMPLE_LOOP);
    add(SampleId::startBeep, "StartBeep", 1);
    add(SampleId::bonusBeep, "BonusBeep", maxSampleCount);
    add(SampleId::broom, "Broom", 1);
    add(SampleId::powerUp, "PowerUp", 1);
    add(SampleId::shipFirePower, "ShipFirePower", 1);
}

void SfxSamples::add(const SampleId sampleId, const char* name, const int maxSampleCount, const SampleFlags flags)
{
    constexpr auto subdir{"sound/"};
    constexpr auto ext{".ogg"};
    std::stringstream ss{};
    ss << subdir << name << ext;
    samples[sampleId] = audioLib.loadSample(ss.str().c_str(), maxSampleCount, flags);
}

void SfxSamples::free()
{
    for (auto sample : samples)
    {
        audioLib.sampleFree(sample.second);
    }
    samples.clear();
}
} // namespace audio
