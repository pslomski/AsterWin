#include "game/GameArea.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace game
{
class TestGameArea : public Test
{
protected:
    GameArea area;
};

TEST_F(TestGameArea, testRandomPosAtEdge)
{
    const auto& pt = area.randomPosAtEdge();
    EXPECT_THAT(pt.x, Ge(area.bounds.x0));
    EXPECT_THAT(pt.x, Le(area.bounds.x1));
    EXPECT_THAT(pt.y, Ge(area.bounds.y0));
    EXPECT_THAT(pt.y, Le(area.bounds.y1));
}
} // namespace game
