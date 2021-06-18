#include <Core/Time.hpp>

namespace te {

const Time Time::Zero;

Time::Time() : Seconds(0) {}

Time::Time(Seconds time) : Seconds(time) {}

Time seconds(float amount) { return Time(Seconds(amount)); }

Time milliseconds(int32_t amount) {
    return Time(MilliSeconds(static_cast<float>(amount) / 1e3));
}

Time microseconds(int64_t amount) {
    return Time(MicroSeconds(static_cast<float>(amount) / 1e6));
}

Clock::Clock() : m_clock(ClockType::now()) {}

Time Clock::getElapsedTime() {
    return std::chrono::duration_cast<Seconds>(ClockType::now() - m_clock);
}

Time Clock::restart() {
    ClockType::time_point now = ClockType::now();
    Time ret = std::chrono::duration_cast<Seconds>(now - m_clock);
    m_clock = now;
    return ret;
}

Clock::ClockType::time_point Clock::now() { return ClockType::now(); }

}  // namespace te
