#ifndef BASE_HPP
#define BASE_HPP

#include <memory>
#include "Core/PlatformDetection.hpp"

namespace te {

template <typename T>
using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> createScope(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> createRef(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}

#ifdef DEBUG_BUILD
	#if defined(TE_PLATFORM_WINDOWS)
		#define TE_DEBUGBREAK() __debugbreak()
	#elif defined(TE_PLATFORM_LINUX)
		#include <signal.h>
		#define TE_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define TE_ENABLE_ASSERTS
#else
	#define TE_DEBUGBREAK()
#endif

#define TE_EXPAND_MACRO(x) x
#define TE_STRINGIFY_MACRO(x) #x

#endif
