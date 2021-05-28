#ifndef GRAPHIC_EXPORT_HPP
#define GRAPHIC_EXPORT_HPP

#include <Config.hpp>

#ifdef GRAPHIC_EXPORTS
#define GRAPHIC_API GL_API_EXPORT
#else
#define GRAPHIC_API GL_API_IMPORT
#endif

#endif
