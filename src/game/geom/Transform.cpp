#include "Transform.hpp"
#include <cmath>

namespace game::geom
{
BoxF transform(const float rotationAngleRad, const types::Vector& translation, const BoxF& seg)
{
    // negate angle for OpenGL coordinate system
    const auto sinalfa = std::sinf(-rotationAngleRad);
    const auto cosalfa = std::cosf(-rotationAngleRad);
    return {
        .x0 = translation.x + seg.x0 * cosalfa + seg.y0 * sinalfa,
        .y0 = translation.y - seg.x0 * sinalfa + seg.y0 * cosalfa,
        .x1 = translation.x + seg.x1 * cosalfa + seg.y1 * sinalfa,
        .y1 = translation.y - seg.x1 * sinalfa + seg.y1 * cosalfa};
}
} // namespace game::geom
