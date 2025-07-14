#define utils_TimeInterval_orig
#include "utils/TimeInterval.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace utils
{
class TestTimeInterval : public Test
{
protected:
    TimeInterval timeInterval;
};

TEST_F(TestTimeInterval, testCreate)
{
    ASSERT_THAT(timeInterval.interval, Eq(1.0f));
}

TEST_F(TestTimeInterval, testGet)
{
    TimeInterval timeInterval{3.0f};
    ASSERT_THAT(timeInterval.get(), Eq(3.0f));
}

TEST_F(TestTimeInterval, testReset)
{
    timeInterval.elapsed = 2.0f;
    timeInterval.reset();
    ASSERT_THAT(timeInterval.get(), Eq(1.0f));
    ASSERT_THAT(timeInterval.elapsed, Eq(0.0f));
}

TEST_F(TestTimeInterval, testReset2)
{
    timeInterval.elapsed = 2.0f;
    timeInterval.reset(4.0f);
    ASSERT_THAT(timeInterval.get(), Eq(4.0f));
    ASSERT_THAT(timeInterval.elapsed, Eq(0.0f));
}

TEST_F(TestTimeInterval, testInc)
{
    ASSERT_FALSE(timeInterval.inc(0.5f));
    ASSERT_TRUE(timeInterval.inc(0.5f));
}

TEST_F(TestTimeInterval, testRatio)
{
    timeInterval.inc(0.5f);
    ASSERT_THAT(timeInterval.ratio(), Eq(0.5f));
}

TEST_F(TestTimeInterval, testRatioWhenIntervalZero)
{
    TimeInterval zeroInterval(0.0f);
    zeroInterval.inc(0.5f);
    ASSERT_DEATH(zeroInterval.ratio(), ".*");
}
} // namespace utils
