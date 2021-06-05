#ifndef LOG_HPP
#define LOG_HPP

#include "Core/Base.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace te {

class Log {
   public:
    static void init();

    static Ref<spdlog::logger> &getCoreLogger();

    static Ref<spdlog::logger> &getClientLogger();

   private:
    static Ref<spdlog::logger> s_coreLogger;
    static Ref<spdlog::logger> s_clientLogger;
};

}  // namespace te

#define TE_CORE_TRACE(...) ::te::Log::getCoreLogger()->trace(__VA_ARGS__)
#define TE_CORE_INFO(...) ::te::Log::getCoreLogger()->info(__VA_ARGS__)
#define TE_CORE_WARN(...) ::te::Log::getCoreLogger()->warn(__VA_ARGS__)
#define TE_CORE_ERROR(...) ::te::Log::getCoreLogger()->error(__VA_ARGS__)
#define TE_CORE_CRITICAL(...) ::te::Log::getCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define TE_TRACE(...) ::te::Log::getClientLogger()->trace(__VA_ARGS__)
#define TE_INFO(...) ::te::Log::getClientLogger()->info(__VA_ARGS__)
#define TE_WARN(...) ::te::Log::getClientLogger()->warn(__VA_ARGS__)
#define TE_ERROR(...) ::te::Log::getClientLogger()->error(__VA_ARGS__)
#define TE_CRITICAL(...) ::te::Log::getClientLogger()->critical(__VA_ARGS__)

#endif
