#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "utils/TimeInterval.hpp"

using namespace testing;

namespace utils
{
class TestTimeInterval : public Test
{
protected:
    TimeInterval timeInteval;
};

TEST_F(TestTimeInterval, testCreate)
{
    ASSERT_THAT(timeInteval.Interval, Eq(1.0f));
}

TEST_F(TestTimeInterval, testCreate2)
{
    TimeInterval timeInteval{2.0f};
    ASSERT_THAT(timeInteval.Interval, Eq(2.0f));
}
} // namespace utils
