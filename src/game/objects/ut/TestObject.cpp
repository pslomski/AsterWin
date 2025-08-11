#define game_objects_Object_orig
#include "game/objects/Object.cpp"

#include "game/Time.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace game::objects
{
constexpr float epsilon = 1.0e-6f;
class TestObject : public Test
{
protected:
    Object object;
};

TEST_F(TestObject, testSetAngleRadGetAngleRad)
{
    object.setAngleRad(pi);
    ASSERT_THAT(object.getAngleRad(), FloatEq(pi));
    ASSERT_THAT(object.getAngleDeg(), FloatEq(180.0f));
}

TEST_F(TestObject, testSetAngleDegGetAngleDeg)
{
    object.setAngleDeg(180.0f);
    ASSERT_THAT(object.getAngleDeg(), FloatEq(180.0f));
    ASSERT_THAT(object.getAngleRad(), FloatEq(pi));
}

TEST_F(TestObject, testSetRotSpeedDegGetRotSpeedDeg)
{
    object.setRotSpeedDeg(180.0f);
    ASSERT_THAT(object.getRotSpeedDeg(), FloatEq(180.0f));
    ASSERT_THAT(object.getRotSpeedRad(), FloatEq(pi));
}

TEST_F(TestObject, testSetRotSpeedRadGetRotSpeedRad)
{
    object.setRotSpeedRad(pi);
    ASSERT_THAT(object.getRotSpeedRad(), FloatEq(pi));
    ASSERT_THAT(object.getRotSpeedDeg(), FloatEq(180.0f));
}

TEST_F(TestObject, distance)
{
    Object object2;
    object.setPosition(0.0f, 0.0f);
    object2.setPosition(1.0f, 0.0f);
    ASSERT_THAT(object.distance(&object2), FloatEq(1.0f));
}

TEST_F(TestObject, transform)
{
    object.setPosition(1.0f, 2.0f);
    object.setAngleRad(pi / 2.0f);
    const BoxF seg{.x0 = 0.0f, .y0 = 0.0f, .x1 = 1.0f, .y1 = 0.0f};
    const auto transformed = object.transform(seg);
    ASSERT_THAT(transformed.x0, FloatNear(1.0f, epsilon));
    ASSERT_THAT(transformed.y0, FloatNear(2.0f, epsilon));
    ASSERT_THAT(transformed.x1, FloatNear(1.0f, epsilon));
    ASSERT_THAT(transformed.y1, FloatNear(3.0f, epsilon));
}
} // namespace game::objects
