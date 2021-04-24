#include <Time.hpp>

const Time Time::Zero;

Time::Time() : MicroSeconds(0) {}

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

Time Clock::restart() {
    Time ret = getElapsedTime();
    m_clock = ClockType::now();
    return ret;
}
