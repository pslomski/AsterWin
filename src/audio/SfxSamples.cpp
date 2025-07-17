#include "SfxSamples.hpp"
#include <sstream>

namespace
{
constexpr audio::Flags FLAG_SAMPLE_LOOP = 0x00000001;
constexpr audio::Flags FLAG_SAMPLE_OVER_VOL = 0x00000002;

DWORD getBassFlags(const audio::Flags flags)
{
    DWORD bassFlags{BASS_SAMPLE_OVER_POS};
    if (flags & FLAG_SAMPLE_LOOP)
    {
        bassFlags |= BASS_SAMPLE_LOOP;
    }
    if (flags & FLAG_SAMPLE_OVER_VOL)
    {
        bassFlags |= BASS_SAMPLE_OVER_VOL;
    }
    return bassFlags;
}
} // namespace

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
        BASS_SampleStop(sample.second);
    }
}

void SfxSamples::init()
{
    if (samples.size() == 0)
    {
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
}

void SfxSamples::addSample(const SampleId sampleId, const char* name, const Flags flags)
{
    constexpr DWORD maxChannelCount = 5;
    constexpr auto subdir{"sound/"};
    constexpr auto ext{".ogg"};
    std::stringstream ss{};
    ss << subdir << name << ext;
    samples[sampleId] = BASS_SampleLoad(FALSE, ss.str().c_str(), 0, 0, maxChannelCount, getBassFlags(flags));
}

void SfxSamples::free()
{
    for (auto sample : samples)
    {
        BASS_SampleFree(sample.second);
    }
    samples.clear();
}
} // namespace audio
