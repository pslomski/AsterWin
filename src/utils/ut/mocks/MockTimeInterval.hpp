#pragma once

#include "common/testing/mocks.hpp"
#if not USE_ORIG(utils_TimeInterval)

#include <gmock/gmock.h>

namespace utils
{
class TimeInterval
{
public:
    TimeInterval() {}
    TimeInterval(const float intervalArg) : interval{intervalArg} {}

    MOCK_METHOD(float, get, (), (const));
    MOCK_METHOD(void, reset, ());
    MOCK_METHOD(void, reset, (const float newInterval));
    MOCK_METHOD(bool, inc, (const float dt));
    MOCK_METHOD(bool, expired, (), (const));
    MOCK_METHOD(float, ratio, (), (const));

    float interval;
    float elapsed{};
};
} // namespace utils

#endif // utils_TimeInterval
