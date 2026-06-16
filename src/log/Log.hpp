#pragma once

#include "log/LogImpl.hpp"

// RFC 5424 severity-tagged logging macros. Each takes a printf-style format
// string plus arguments; the MSGID field defaults to the NILVALUE ("-").
#define LOG_EMERG(...)   ::logging::write(::logging::Severity::Emergency, "-", __VA_ARGS__)
#define LOG_ALERT(...)   ::logging::write(::logging::Severity::Alert,     "-", __VA_ARGS__)
#define LOG_CRIT(...)    ::logging::write(::logging::Severity::Critical,  "-", __VA_ARGS__)
#define LOG_ERR(...)     ::logging::write(::logging::Severity::Error,     "-", __VA_ARGS__)
#define LOG_WARNING(...) ::logging::write(::logging::Severity::Warning,   "-", __VA_ARGS__)
#define LOG_NOTICE(...)  ::logging::write(::logging::Severity::Notice,    "-", __VA_ARGS__)
#define LOG_INFO(...)    ::logging::write(::logging::Severity::Info,      "-", __VA_ARGS__)
#define LOG_DEBUG(...)   ::logging::write(::logging::Severity::Debug,     "-", __VA_ARGS__)

// Generic form for when the RFC 5424 MSGID field should be set, e.g.
// LOG_MSG(::logging::Severity::Info, "AUDIO", "device opened: %s", name).
#define LOG_MSG(severity, msgid, ...) ::logging::write(severity, msgid, __VA_ARGS__)
