#include "game/types/Color.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace game::objects
{
constexpr auto r{0.1f};
constexpr auto g{0.2f};
constexpr auto b{0.3f};

class TestColor : public Test
{
protected:
    Color color;

    void verify(const Color& colorArg, const Color& expected)
    {
        ASSERT_THAT(colorArg.red, Eq(expected.red));
        ASSERT_THAT(colorArg.green, Eq(expected.green));
        ASSERT_THAT(colorArg.blue, Eq(expected.blue));
    }
};

TEST_F(TestColor, testSet)
{
    color.set({r, g, b});
    verify(color, {r, g, b});
}

TEST_F(TestColor, testMultiply)
{
    color.set({r, g, b});
    const auto color2 = color * 2.0f;
    ASSERT_THAT(color2.red, Eq(r * 2.0f));
    ASSERT_THAT(color2.green, Eq(g * 2.0f));
    ASSERT_THAT(color2.blue, Eq(b * 2.0f));
}
} // namespace game::objects
