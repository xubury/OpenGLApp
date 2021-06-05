#ifndef GL_CONTEXT_HPP
#define GL_CONTEXT_HPP

#include "Core/Export.hpp"
#include "glbinding/glbinding.h"

namespace te {

class TE_API GLContext {
   public:
    static void init(glbinding::GetProcAddress procAddr);
};

}  // namespace te

#endif
