#define game_objects_Object_orig
#include "game/objects/Object.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "game/geom/Transform.cpp"

using namespace testing;

namespace game::objects
{
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
} // namespace game::objects
