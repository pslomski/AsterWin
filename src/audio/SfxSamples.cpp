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
    addSample(SampleId::shipEngine, "ShipEngine", FLAG_SAMPLE_LOOP);
    addSample(SampleId::shipFire, "ShipFire");
    addSample(SampleId::shipCrash, "ShipCrash");
    addSample(SampleId::asterCrash1, "AsterCrash1");
    addSample(SampleId::asterCrash2, "AsterCrash2");
    addSample(SampleId::asterCrash3, "AsterCrash3");
    addSample(SampleId::ufoEngine, "UfoEngine", FLAG_SAMPLE_LOOP);
    addSample(SampleId::startBeep, "StartBeep");
    addSample(SampleId::bonusBeep, "BonusBeep");
    addSample(SampleId::broom, "Broom");
    addSample(SampleId::powerUp, "PowerUp");
    addSample(SampleId::shipFirePower, "ShipFirePower");
}

void SfxSamples::addSample(const SampleId sampleId, const char* name, const SampleFlags flags)
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
