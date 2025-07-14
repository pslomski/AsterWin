#pragma once

#include "common/testing/mocks.hpp"
#if not USE_ORIG(utils_TimeInterval)

#include <gmock/gmock.h>
#include "game/types/Types.hpp"

namespace utils
{
class TimeInterval
{
public:
    TimeInterval() {}
    TimeInterval(const Float intervalArg) {}

    MOCK_METHOD(Float, get, (), (const));
    MOCK_METHOD(void, reset, ());
    MOCK_METHOD(void, reset, (const Float newInterval));
    MOCK_METHOD(void, setExpired, ());
    MOCK_METHOD(bool, inc, (const Float dt));
    MOCK_METHOD(Float, ratio, (), (const));

    Float interval;
    Float elapsed{};
};
} // namespace utils

#endif // utils_TimeInterval
