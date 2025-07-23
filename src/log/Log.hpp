#pragma once

#include "log/LogImpl.hpp"

#define LOG_INF(...) logInfo(__VA_ARGS__)
#define LOG_ERR(...) logError(__VA_ARGS__)
