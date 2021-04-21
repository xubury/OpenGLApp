#ifndef TIME_HPP
#define TIME_HPP

#include <chrono>

using MicroSeconds = std::chrono::microseconds;
using MilliSeconds = std::chrono::milliseconds;
using Seconds = std::chrono::seconds;

class Time : public MicroSeconds {
   public:
    template <typename T, typename R>
    static Time as(R time) {
        return Time(
            std::chrono::duration_cast<T>(std::chrono::duration<R>(time)));
    }
    Time(MicroSeconds time) : MicroSeconds(time) {}

    Time(MilliSeconds time) : MicroSeconds(time) {}

    Time(Seconds time)
        : MicroSeconds(std::chrono::duration_cast<MicroSeconds>(time)) {}

    template <typename T>
    T as() {
        return std::chrono::duration_cast<T>(*this);
    }
};

class Clock {
   public:
    using ClockType = std::chrono::steady_clock;
    Clock() {}

    Time getElapsedTime() {
        return std::chrono::duration_cast<MicroSeconds>(ClockType::now() -
                                                        m_clock);
    }

    void restart() { m_clock = ClockType::now(); }

   private:
    std::chrono::steady_clock::time_point m_clock;
};

#endif
