#ifndef GL_CONTEXT_HPP
#define GL_CONTEXT_HPP

#include "Graphic/OpenGL.hpp"

namespace te {

class GLContext {
   public:
    static void init(glbinding::GetProcAddress procAddr);
};

}  // namespace te

#endif
