#define game_objects_Object_orig
#define game_geom_LineIntersection_orig
#include "game/geom/Bounds.cpp"
#include "game/geom/CheckCollision.cpp"
#include "game/geom/LineIntersection.cpp"
#include "game/geom/Transform.cpp"
#include "game/objects/Object.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace game::geom
{
const auto squarePolygon{
    PointsF{{.x = 1.0f, .y = 1.0f}, {.x = -1.0f, .y = 1.0f}, {.x = -1.0f, .y = -1.0f}, {.x = 1.0f, .y = -1.0f}}};

class TestCheckCollision : public Test
{
protected:
    void setupObject(objects::Object& obj, const GeometryType geomType, const PointF& pos)
    {
        obj.geometryType = geomType;
        obj.state.pos = pos;
        obj.verts.clear();
        if (geomType == GeometryType::Polyg)
        {
            obj.verts = squarePolygon;
            obj.bounds = geom::calcBounds(obj.verts);
        }
    }

    objects::Object obj1;
    objects::Object obj2;
};

// TODO: add more tests for different geometries and positions
TEST_F(TestCheckCollision, checkCollision)
{
    setupObject(obj1, GeometryType::Polyg, {0.0f, 0.0f});
    setupObject(obj2, GeometryType::Polyg, {0.5f, 0.5f});
    ASSERT_TRUE(geom::checkCollision(&obj1, &obj2));
}
} // namespace game::geom
