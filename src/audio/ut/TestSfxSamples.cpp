#define audio_SfxSamples_orig
#include "audio/SfxSamples.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace audio
{
class TestSfxSamples : public Test
{
protected:
    SfxSamples sfxSamples;
};

TEST_F(TestSfxSamples, testInit)
{
    constexpr auto sampleCount = 12;
    auto* const dummy = reinterpret_cast<MIX_Audio*>(1);
    EXPECT_CALL(audioLib, loadSample(_)).Times(sampleCount).WillRepeatedly(Return(dummy)).RetiresOnSaturation();
    EXPECT_CALL(audioLib, freeSample(dummy)).Times(sampleCount).RetiresOnSaturation();
    sfxSamples.init();
}
} // namespace audio
