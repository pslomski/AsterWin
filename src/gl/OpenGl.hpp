#pragma once

// Portable OpenGL header include (Windows/Linux use GL/, macOS uses the OpenGL framework).
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
