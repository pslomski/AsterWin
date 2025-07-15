#pragma once

#include "game/objects/Object.hpp"
#include "utils/TimeInterval.hpp"

class TempObject : public Object
{
public:
    TempObject();
    explicit TempObject(const Float lifeTime);
    bool expired() override;
    void setExpired();

    utils::TimeInterval lifeTime;
};
