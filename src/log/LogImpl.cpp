#include "log/LogImpl.hpp"

#include <SDL3/SDL_time.h>

#include <cstddef>
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <process.h> // _getpid
#else
#include <unistd.h> // getpid
#endif

namespace logging
{
namespace
{

// RFC 5424 (section 6.2.1, Table 1): facility 1 = "user-level messages", the
// correct bucket for an ordinary userland application.
constexpr int FACILITY = 1;

// APP-NAME (RFC 5424 section 6.2.5): up to 48 PRINTUSASCII characters.
constexpr const char* APP_NAME = "Asteroids";

// HOSTNAME (RFC 5424 section 6.2.4). Resolved once; falls back to the NILVALUE.
const char* hostName()
{
    static const char* const host = []() -> const char*
    {
        if (const char* h = std::getenv("COMPUTERNAME")) return h; // Windows
        if (const char* h = std::getenv("HOSTNAME")) return h;     // POSIX shells
        return "-";
    }();
    return host;
}

// PROCID (RFC 5424 section 6.2.6). Stringified process id, computed once.
const char* procId()
{
    static char buf[16] = {};
    if (buf[0] == '\0')
    {
#ifdef _WIN32
        std::snprintf(buf, sizeof(buf), "%d", _getpid());
#else
        std::snprintf(buf, sizeof(buf), "%d", getpid());
#endif
    }
    return buf;
}

// TIMESTAMP (RFC 5424 section 6.2.3): an RFC 3339 date-time with up to six
// fractional-second digits. Written to out as local time with a numeric offset.
void formatTimestamp(char* out, std::size_t size)
{
    SDL_Time now;
    SDL_DateTime dt;
    if (!SDL_GetCurrentTime(&now) || !SDL_TimeToDateTime(now, &dt, true))
    {
        std::snprintf(out, size, "-"); // NILVALUE
        return;
    }

    // Real-world UTC offsets are whole minutes, and RFC 3339's TIME-NUMOFFSET
    // only expresses "+HH:MM". SDL's Windows backend can be off by up to a
    // second (it truncates a sub-second difference), so round to the nearest
    // minute to avoid stray values like "+01:59" for a true "+02:00".
    const int offset = dt.utc_offset; // seconds east of UTC
    const int totalMinutes = (offset + (offset < 0 ? -30 : 30)) / 60;
    const char sign = totalMinutes < 0 ? '-' : '+';
    const int absMinutes = totalMinutes < 0 ? -totalMinutes : totalMinutes;
    const int micros = dt.nanosecond / 1000;

    std::snprintf(out, size, "%04d-%02d-%02dT%02d:%02d:%02d.%06d%c%02d:%02d",
        dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second, micros,
        sign, absMinutes / 60, absMinutes % 60);
}

} // namespace

void emit(Severity severity, const char* msgid, const char* msg)
{
    // PRI = FACILITY * 8 + SEVERITY (RFC 5424 section 6.2.1).
    const int pri = FACILITY * 8 + static_cast<int>(severity);

    char timestamp[40];
    formatTimestamp(timestamp, sizeof(timestamp));

    // HEADER SP STRUCTURED-DATA SP MSG (RFC 5424 section 6), with VERSION = 1 and
    // STRUCTURED-DATA = NILVALUE:
    // <PRI>VERSION TIMESTAMP HOSTNAME APP-NAME PROCID MSGID SD MSG
    std::printf("<%d>1 %s %s %s %s %s - %s\n", pri, timestamp, hostName(),
        APP_NAME, procId(), (msgid && *msgid) ? msgid : "-", msg);
}

} // namespace logging
