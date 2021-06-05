#ifndef GRAPHIC_EXPORT_HPP
#define GRAPHIC_EXPORT_HPP

#include "Core/Config.hpp"

#ifdef TE_EXPORT
#define TE_API TE_API_EXPORT
#else
#define TE_API TE_API_IMPORT
#endif

#endif
