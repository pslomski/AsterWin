#pragma once

#include <cstdint>
#include <string>

namespace common
{
struct HighScore
{
    bool operator<(const HighScore& other) { return score > other.score; }

    std::string playerName{};
    uint32_t score{};
};
} // namespace common
