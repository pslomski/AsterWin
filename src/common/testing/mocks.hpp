#pragma once

#if UNIT_TEST

#define USE_ORIG(name) defined(name##_orig)

#else

#define USE_ORIG(name) true

#endif // UNIT_TEST
