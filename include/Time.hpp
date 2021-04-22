#ifndef TIME_HPP
#define TIME_HPP

#include <chrono>

using MicroSeconds = std::chrono::microseconds;
using MilliSeconds = std::chrono::milliseconds;
using Seconds = std::chrono::seconds;

class Time : public MicroSeconds {
   public:
    Time(MicroSeconds time);

    template <typename T>
    T as() {
        return std::chrono::duration_cast<T>(*this);
    }
};

class Clock {
   public:
    using ClockType = std::chrono::high_resolution_clock;

    Time getElapsedTime();

    void restart();

   private:
    ClockType::time_point m_clock;
};

Time seconds(float amount);

Time milliseconds(int32_t amount);

Time microseconds(int64_t amount);

#endif
