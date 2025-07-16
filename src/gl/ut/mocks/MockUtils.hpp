#pragma once

#include "common/testing/mocks.hpp"
#if not USE_ORIG(gl_Utils)

#include "game/types/Color.hpp"

inline void setGlColor(const Color& color) {}

inline void setGlColor(const float gray) {}

#endif // gl_Utils
