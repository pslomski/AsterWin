#include "game/Math.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace game
{
class TestMath : public Test
{
};

TEST_F(TestMath, testDegToRad)
{
    ASSERT_THAT(degToRad(0.0f), FloatEq(0.0f));
    ASSERT_THAT(degToRad(180.0f), FloatEq(pi));
    ASSERT_THAT(degToRad(360.0f), FloatEq(2 * pi));
    ASSERT_THAT(degToRad(90.0f), FloatEq(pi / 2));
    ASSERT_THAT(degToRad(-90.0f), FloatEq(-pi / 2));
}

TEST_F(TestMath, testRadToDeg)
{
    ASSERT_THAT(radToDeg(0.0f), FloatEq(0.0f));
    ASSERT_THAT(radToDeg(pi), FloatEq(180.0f));
    ASSERT_THAT(radToDeg(2 * pi), FloatEq(360.0f));
    ASSERT_THAT(radToDeg(pi / 2), FloatEq(90.0f));
    ASSERT_THAT(radToDeg(-pi / 2), FloatEq(-90.0f));
}

TEST_F(TestMath, testNormalizeAngleRad)
{
    ASSERT_THAT(normalizeAngleRad(0.0f), FloatEq(0.0f));
    ASSERT_THAT(normalizeAngleRad(pi), FloatEq(pi));
    ASSERT_THAT(normalizeAngleRad(2 * pi), FloatEq(0.0f));
    ASSERT_THAT(normalizeAngleRad(2 * pi + 1.0f), FloatEq(1.0f));
    ASSERT_THAT(normalizeAngleRad(-pi), FloatEq(pi));
    ASSERT_THAT(normalizeAngleRad(-2 * pi), FloatEq(0.0f));
}
} // namespace game
