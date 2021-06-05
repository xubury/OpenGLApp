#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Core/PlatformDetection.hpp"

#if !defined(TE_STATIC)

#if defined(TE_PLATFORM_WINDOWS)

// Windows compilers need specific (and different) keywords for export and
// import
#define TE_API_EXPORT __declspec(dllexport)
#define TE_API_IMPORT __declspec(dllimport)

// For Visual C++ compilers, we also need to turn off this annoying C4251
// warning
#ifdef _MSC_VER

#pragma warning(disable : 4251)

#endif

#else  // Linux, FreeBSD, Mac OS X

#if __GNUC__ >= 4

// GCC 4 has special keywords for showing/hidding symbols,
// the same keyword is used for both importing and exporting
#define TE_API_EXPORT __attribute__((__visibility__("default")))
#define TE_API_IMPORT __attribute__((__visibility__("default")))

#else

// GCC < 4 has no mechanism to explicitely hide symbols, everything's exported
#define TE_API_EXPORT
#define TE_API_IMPORT

#endif

#endif

#else

// Static build doesn't need import/export macros
#define TE_API_EXPORT
#define TE_API_IMPORT

#endif

#endif
