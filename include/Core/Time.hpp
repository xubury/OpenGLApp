#ifndef TIME_HPP
#define TIME_HPP

#include <chrono>
#include "Core/Export.hpp"

namespace te {

using MicroSeconds = std::chrono::duration<float, std::micro>;
using MilliSeconds = std::chrono::duration<float, std::milli>;
using Seconds = std::chrono::duration<float>;

class TE_API Time : public Seconds {
   public:
    Time();

    Time(Seconds time);

    template <typename T>
    T as() {
        return std::chrono::duration_cast<T>(*this);
    }

    static const Time Zero;
};

class TE_API Clock {
   public:
    Clock();

    using ClockType = std::chrono::high_resolution_clock;

    Time getElapsedTime();

    Time restart();

    static ClockType::time_point now();

   private:
    ClockType::time_point m_clock;
};

Time TE_API seconds(float amount);

Time TE_API milliseconds(int32_t amount);

Time TE_API microseconds(int64_t amount);

}  // namespace te

#endif
