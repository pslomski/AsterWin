#include "TempObject.hpp"

namespace game::objects
{
TempObject::TempObject() : Object() {}

TempObject::TempObject(const Float lifeTime) : lifeTime(lifeTime) {}

bool TempObject::expired()
{
    return lifeTime.inc(dt);
}
} // namespace game::objects
