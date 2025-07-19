#pragma once

#include <cstdint>

namespace audio
{
using SampleFlags = uint32_t;

inline constexpr SampleFlags FLAG_SAMPLE_LOOP = 0x00000001;
inline constexpr SampleFlags FLAG_SAMPLE_OVER_VOL = 0x00000002;
} // namespace audio
