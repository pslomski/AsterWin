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
    EXPECT_CALL(audioLib, loadSample(_, _, _)).Times(sampleCount).WillRepeatedly(Return(1)).RetiresOnSaturation();
    EXPECT_CALL(audioLib, sampleFree(1)).Times(sampleCount).RetiresOnSaturation();
    sfxSamples.init();
}
} // namespace audio
