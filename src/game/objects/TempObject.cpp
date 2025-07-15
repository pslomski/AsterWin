#include "TempObject.hpp"

TempObject::TempObject() : Object() {}

TempObject::TempObject(const Float lifeTime) : lifeTime(lifeTime) {}

bool TempObject::expired()
{
    return lifeTime.inc(dt);
}

void TempObject::setExpired()
{
    lifeTime.setExpired();
}
