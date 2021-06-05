#include "Core/Assert.hpp"
#include "Graphic/GLContext.hpp"

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
