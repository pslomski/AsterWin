#include "game/geom/Transform.cpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "game/Consts.hpp"

using namespace testing;

namespace game::geom
{
constexpr float epsilon = 1.0e-6f;

TEST(TestTransform, transform)
{
    const types::Vector translation{.x = 1.0f, .y = 2.0f};
    const auto angleRad{pi / 2.0f};
    const BoxF seg{.x0 = 0.0f, .y0 = 0.0f, .x1 = 1.0f, .y1 = 0.0f};
    const auto transformed = transform(angleRad, translation, seg);
    ASSERT_THAT(transformed.x0, FloatNear(1.0f, epsilon));
    ASSERT_THAT(transformed.y0, FloatNear(2.0f, epsilon));
    ASSERT_THAT(transformed.x1, FloatNear(1.0f, epsilon));
    ASSERT_THAT(transformed.y1, FloatNear(3.0f, epsilon));
}
} // namespace game::geom
