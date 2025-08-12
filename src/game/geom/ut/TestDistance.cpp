#include "game/geom/Distance.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace game::geom
{
TEST(TestDistance, distance)
{
    types::Position pos1{0.0f, 0.0f};
    types::Position pos2{1.0f, 0.0f};
    ASSERT_THAT(distance(pos1, pos2), FloatEq(1.0f));
}
} // namespace game::geom
