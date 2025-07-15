#pragma once

#include "game/objects/Object.hpp"
#include "utils/TimeInterval.hpp"

class TempObject : public Object
{
public:
    TempObject();
    explicit TempObject(const Float lifeTime);
    virtual bool expired();
    void setExpired();

    utils::TimeInterval lifeTime;
};
