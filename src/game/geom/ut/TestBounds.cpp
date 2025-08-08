#include "game/geom/Bounds.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace game::geom
{
const auto squarePolygon{
    PointsF{{.x = 1.0f, .y = 1.0f}, {.x = -1.0f, .y = 1.0f}, {.x = -1.0f, .y = -1.0f}, {.x = 1.0f, .y = -1.0f}}};
const auto r{std::sqrtf(2.0f) * 1.05f};

class TestBounds : public Test
{
protected:
    void setupObjects(const PointsF& pointsArg) { points = pointsArg; }

    PointsF points;
};

TEST_F(TestBounds, calcBounds)
{
    setupObjects(squarePolygon);
    const auto bounds = calcBounds(points);
    ASSERT_THAT(bounds.x0, FloatEq(-r));
    ASSERT_THAT(bounds.y0, FloatEq(-r));
    ASSERT_THAT(bounds.x1, FloatEq(r));
    ASSERT_THAT(bounds.y1, FloatEq(r));
}
} // namespace game::geom
