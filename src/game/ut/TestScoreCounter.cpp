#include <gtest/gtest.h>
#include "game/ScoreCounter.hpp"

using namespace testing;

namespace game
{
class TestScoreCounter : public Test
{
protected:
    ScoreCounter::OnIncrementLives onIncrementLives;
    ScoreCounter counter{onIncrementLives};
};

TEST_F(TestScoreCounter, shouldCreate)
{
    ASSERT_EQ(0, counter.get());
}

TEST_F(TestScoreCounter, shouldInc)
{
    counter.inc(2);
    counter.inc(3);
    ASSERT_EQ(5, counter.get());
}

TEST_F(TestScoreCounter, shouldReset)
{
    counter.inc(10);
    counter.reset();
    ASSERT_EQ(0, counter.get());
}
} // namespace game
