#define game_objects_Object_orig
#include "game/objects/Object.cpp"

#include "game/Time.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace game::objects
{
class TestObject : public Test
{
protected:
    Object object;
};

TEST_F(TestObject, distance)
{
    Object object2;
    object.setXY(0.0f, 0.0f);
    object2.setXY(1.0f, 0.0f);
    ASSERT_THAT(object.distance(&object2), Eq(1.0f));
}
} // namespace game::objects
