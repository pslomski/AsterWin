#pragma once

#include <cstdio>

namespace logging
{

// RFC 5424 (section 6.2.1, Table 2) severity levels. A lower numerical value
// denotes a higher severity.
enum class Severity
{
    Emergency = 0, // system is unusable
    Alert     = 1, // action must be taken immediately
    Critical  = 2, // critical conditions
    Error     = 3, // error conditions
    Warning   = 4, // warning conditions
    Notice    = 5, // normal but significant condition
    Info      = 6, // informational messages
    Debug     = 7, // debug-level messages
};

// Emits a single RFC 5424 SYSLOG line for an already-formatted MSG. A null or
// empty msgid is rendered as the NILVALUE ("-").
void emit(Severity severity, const char* msgid, const char* msg);

// Formats a printf-style message into a fixed buffer and emits it as one line.
template <typename... Args>
void write(Severity severity, const char* msgid, const char* fmt, Args... args)
{
    // RFC 5424 (section 6.1) requires support for at least 480 octets and
    // recommends 2048; longer messages are safely truncated by snprintf.
    char msg[2048];
    std::snprintf(msg, sizeof(msg), fmt, args...);
    emit(severity, msgid, msg);
}

} // namespace logging
