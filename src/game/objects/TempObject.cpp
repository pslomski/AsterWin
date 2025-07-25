#include "TempObject.hpp"
#include "game/Time.hpp"

namespace game::objects
{
TempObject::TempObject() : Object() {}

TempObject::TempObject(const Float lifeTimeArg) : Object(), lifeTime{lifeTimeArg} {}

bool TempObject::expired()
{
    return lifeTime.inc(time.dt);
}
} // namespace game::objects
