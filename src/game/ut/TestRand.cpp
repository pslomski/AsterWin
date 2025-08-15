#include "game/Rand.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace game
{
class TestRand : public Test
{
};

TEST_F(TestRand, isInRange)
{
    for (int i = 0; i < 100; ++i)
    {
        int value = randi(10);
        ASSERT_THAT(value, Ge(0));
        ASSERT_THAT(value, Lt(10));
    }
}
} // namespace game
