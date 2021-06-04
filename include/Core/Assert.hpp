#ifndef ASSERT_HPP
#define ASSERT_HPP

#include <filesystem>

#include "Core/Log.hpp"
#include "Core/PlatformDetection.hpp"

#ifdef TE_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and
// "NO_MSG" and provide support for custom formatting by concatenating the
// formatting string instead of having the format inside the default message
#define TE_INTERNAL_ASSERT_IMPL(type, check, msg, ...) \
    {                                                  \
        if (!(check)) {                                \
            TE##type##ERROR(msg, __VA_ARGS__);         \
            TE_DEBUGBREAK();                           \
        }                                              \
    }
#define TE_INTERNAL_ASSERT_WITH_MSG(type, check, ...) \
    TE_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define TE_INTERNAL_ASSERT_NO_MSG(type, check)            \
    TE_INTERNAL_ASSERT_IMPL(                              \
        type, check, "Assertion '{0}' failed at {1}:{2}", \
        TE_STRINGIFY_MACRO(check),                        \
        std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define TE_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define TE_INTERNAL_ASSERT_GET_MACRO(...)              \
    TE_EXPAND_MACRO(TE_INTERNAL_ASSERT_GET_MACRO_NAME( \
        __VA_ARGS__, TE_INTERNAL_ASSERT_WITH_MSG, TE_INTERNAL_ASSERT_NO_MSG))

// Currently accepts at least the condition and one additional parameter (the
// message) being optional
#define TE_ASSERT(...) \
    TE_EXPAND_MACRO(TE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__))
#define TE_CORE_ASSERT(...) \
    TE_EXPAND_MACRO(        \
        TE_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__))
#else
#define TE_ASSERT(...)
#define TE_CORE_ASSERT(...)

#endif

#endif
