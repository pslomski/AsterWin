#include "TempObject.hpp"

namespace game::objects
{
TempObject::TempObject() : Object() {}

TempObject::TempObject(const Float lifeTimeArg) : Object(), lifeTime{lifeTimeArg} {}

bool TempObject::expired()
{
    return lifeTime.inc(dt);
}
} // namespace game::objects
