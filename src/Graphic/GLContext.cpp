#include "Graphic/GLContext.hpp"
#include "Graphic/OpenGL.hpp"
#include "Core/Assert.hpp"

namespace te {

void GLContext::init(glbinding::GetProcAddress procAddr) {
    glbinding::initialize(procAddr);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // MSAA
    glEnable(GL_MULTISAMPLE);
}

}  // namespace te
