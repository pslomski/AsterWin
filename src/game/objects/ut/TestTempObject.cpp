#include "game/objects/TempObject.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace game::objects
{
class TestTempObject : public Test
{
protected:
    TempObject object;
};

TEST_F(TestTempObject, testExpired)
{
    ASSERT_FALSE(object.expired());
}
} // namespace game::objects
