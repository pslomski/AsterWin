#pragma once

#include "game/objects/Object.hpp"
#include "utils/TimeInterval.hpp"

namespace game::objects
{
class TempObject : public Object
{
public:
    TempObject();
    explicit TempObject(const Float lifeTimeArg);
    virtual bool expired();

    utils::TimeInterval lifeTime;
};
} // namespace game::objects
