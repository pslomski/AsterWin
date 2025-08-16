#include "game/Rand.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace game
{
class TestRand : public Test
{
};

TEST_F(TestRand, isInIntRange)
{
    for (int i = 0; i < 100; ++i)
    {
        int value = rand(10);
        ASSERT_THAT(value, Ge(0));
        ASSERT_THAT(value, Lt(10));
    }
}

TEST_F(TestRand, isInFloatMinMax)
{
    const auto min{-2.0f};
    const auto max{10.0f};
    for (int i = 0; i < 100; ++i)
    {
        const auto value = rand(min, max);
        ASSERT_THAT(value, Ge(min));
        ASSERT_THAT(value, Le(max));
    }
}
} // namespace game
