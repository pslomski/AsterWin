#define ui_Settings_orig
#include "ui/Settings.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace ui
{
class TestSettings : public Test
{
protected:
    Settings settings;
};

TEST_F(TestSettings, testDefaultValues)
{
    ASSERT_THAT(settings.musicVol, Eq(5));
    ASSERT_THAT(settings.soundVol, Eq(5));
}

TEST_F(TestSettings, testSaveAndLoad)
{
    settings.musicVol = 6;
    settings.soundVol = 7;
    settings.save();
    settings.load();
    ASSERT_THAT(settings.musicVol, Eq(6));
    ASSERT_THAT(settings.soundVol, Eq(7));
}
} // namespace ui
