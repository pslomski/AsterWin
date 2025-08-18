#define game_geom_LineIntersection_orig
#define game_geom_PolygWithPointCheck_orig
#define game_objects_Object_orig
#include "game/geom/PolygWithPointCheck.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "game/geom/LineIntersection.cpp"
#include "game/geom/PointInPolygon.cpp"
#include "game/geom/Transform.cpp"
#include "game/objects/Object.cpp"
#include "game/types/Point.hpp"

using namespace testing;

namespace game::geom
{
const auto squarePolygon{
    PointsF{{.x = 1.0f, .y = 1.0f}, {.x = -1.0f, .y = 1.0f}, {.x = -1.0f, .y = -1.0f}, {.x = 1.0f, .y = -1.0f}}};

struct PolygWithPointCheckParam
{
    PointF point{};
    PointsF polyg{};
    bool expected{};

    friend std::ostream& operator<<(std::ostream& os, const PolygWithPointCheckParam& param)
    {
        os << "point:" << param.point.x << "," << param.point.y << ", polygon npol:" << param.polyg.size()
           << ", expected: " << param.expected;
        return os;
    }
};

class TestPolygWithPointCheck : public TestWithParam<PolygWithPointCheckParam>
{
protected:
    void setupObjects(const PointF& pointArg, const PointsF& polygArg)
    {
        point.state.pos = pointArg;
        point.statep.pos = pointArg;
        polyg.verts = polygArg;
    }

    objects::Object point;
    objects::Object polyg;
};

TEST_P(TestPolygWithPointCheck, evaluate)
{
    setupObjects(GetParam().point, GetParam().polyg);
    ASSERT_THAT(checkPolygWithPoint(&point, &polyg), Eq(GetParam().expected));
}

INSTANTIATE_TEST_SUITE_P(
    ,
    TestPolygWithPointCheck,
    Values(
        PolygWithPointCheckParam{.point = {0.0f, 0.0f}, .polyg = squarePolygon, .expected = true},
        PolygWithPointCheckParam{.point = {2.0f, 0.0f}, .polyg = squarePolygon, .expected = false}));
} // namespace game::geom
