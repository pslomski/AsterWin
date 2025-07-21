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
    add(SampleId::shipEngine, "ShipEngine", FLAG_SAMPLE_LOOP);
    add(SampleId::shipFire, "ShipFire");
    add(SampleId::shipCrash, "ShipCrash");
    add(SampleId::asterCrash1, "AsterCrash1");
    add(SampleId::asterCrash2, "AsterCrash2");
    add(SampleId::asterCrash3, "AsterCrash3");
    add(SampleId::ufoEngine, "UfoEngine", FLAG_SAMPLE_LOOP);
    add(SampleId::startBeep, "StartBeep");
    add(SampleId::bonusBeep, "BonusBeep");
    add(SampleId::broom, "Broom");
    add(SampleId::powerUp, "PowerUp");
    add(SampleId::shipFirePower, "ShipFirePower");
}

void SfxSamples::add(const SampleId sampleId, const char* name, const SampleFlags flags)
{
    constexpr auto subdir{"sound/"};
    constexpr auto ext{".ogg"};
    std::stringstream ss{};
    ss << subdir << name << ext;
    samples[sampleId] = audioLib.loadSample(ss.str().c_str(), flags);
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
