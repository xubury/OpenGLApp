#include <Time.hpp>

Time::Time(MicroSeconds time) : MicroSeconds(time) {}

Time seconds(float amount) {
    return Time(std::chrono::duration<int64_t, std::micro>(
        static_cast<int64_t>(amount * 1000000)));
}

Time milliseconds(int32_t amount) {
    return Time(std::chrono::duration<int64_t, std::micro>(
        static_cast<int64_t>(amount * 1000)));
}

Time microseconds(int64_t amount) {
    return Time(std::chrono::duration<int64_t, std::micro>(amount));
}

Time Clock::getElapsedTime() {
    return std::chrono::duration_cast<MicroSeconds>(ClockType::now() - m_clock);
}

void Clock::restart() { m_clock = ClockType::now(); }
