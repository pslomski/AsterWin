#include "game/ScoreCounter.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

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
    ASSERT_THAT(counter.get(), Eq(0));
}

TEST_F(TestScoreCounter, shouldInc)
{
    counter.inc(2);
    counter.inc(3);
    ASSERT_THAT(counter.get(), Eq(5));
}

TEST_F(TestScoreCounter, shouldReset)
{
    counter.inc(10);
    counter.reset();
    ASSERT_THAT(counter.get(), Eq(0));
}
} // namespace game
