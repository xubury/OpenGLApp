#ifndef TIME_HPP
#define TIME_HPP

#include <chrono>

namespace te {

using MicroSeconds = std::chrono::duration<float, std::micro>;
using MilliSeconds = std::chrono::duration<float, std::milli>;
using Seconds = std::chrono::duration<float>;

class Time : public Seconds {
   public:
    Time();

    Time(Seconds time);

    template <typename T>
    T as() {
        return std::chrono::duration_cast<T>(*this);
    }

    static const Time Zero;
};

class Clock {
   public:
    Clock();

    using ClockType = std::chrono::high_resolution_clock;

    Time getElapsedTime();

    Time restart();

    static ClockType::time_point now();

   private:
    ClockType::time_point m_clock;
};

Time seconds(float amount);

Time milliseconds(int32_t amount);

Time microseconds(int64_t amount);

}  // namespace te

#endif
