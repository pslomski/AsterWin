#include "Time.hpp"
#include <chrono>

namespace game
{
double Time::getCurrentTime() const
{
    using namespace std::chrono;
    auto now = steady_clock::now().time_since_epoch();
    return duration<double>(now).count();
}
} // namespace game
