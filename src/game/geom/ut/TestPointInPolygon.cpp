#include "game/geom/PointInPolygon.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace utils
{
struct TestIsPointInPolygonParam
{
    size_t npol{};
    PointsF points{};
    float x{};
    float y{};
    bool expected{};
};

class TestIsPointInPolygon : public TestWithParam<TestIsPointInPolygonParam>
{
};

TEST_P(TestIsPointInPolygon, evaluate)
{
    ASSERT_THAT(
        isPointInPolygon(GetParam().npol, GetParam().points, GetParam().x, GetParam().y), Eq(GetParam().expected));
}

INSTANTIATE_TEST_CASE_P(
    ,
    TestIsPointInPolygon,
    Values(
        TestIsPointInPolygonParam{
            .npol = 4,
            .points = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}},
            .x = 0.0,
            .y = 0.0,
            .expected = true},
        TestIsPointInPolygonParam{
            .npol = 4,
            .points = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}},
            .x = 0.999999,
            .y = 0.999999,
            .expected = true},
        TestIsPointInPolygonParam{
            .npol = 4,
            .points = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}},
            .x = 1.0,
            .y = 1.0,
            .expected = false},
        TestIsPointInPolygonParam{
            .npol = 4,
            .points = {{1, 1}, {-1, 1}, {-1, -1}, {1, -1}},
            .x = -1.0,
            .y = -1.0,
            .expected = true}));
} // namespace utils
