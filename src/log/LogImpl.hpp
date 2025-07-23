#pragma once
#include <cstdio>

template <typename... Args>
void log(const char* prefix, const char* fmt, Args... args)
{
    std::printf("[%s] ", prefix);
    std::printf(fmt, args...);
    std::printf("\n");
}

template <typename... Args>
void logInfo(const char* fmt, Args... args)
{
    log("INF:", fmt, args...);
}

template <typename... Args>
void logError(const char* fmt, Args... args)
{
    log("ERR:", fmt, args...);
}
