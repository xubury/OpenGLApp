#include "Graphic/GLContext.hpp"
#include "Graphic/OpenGL.hpp"
#include "Core/Assert.hpp"

namespace te {

#ifdef DEBUG_BUILD
static void OpenGLMessageCallback(GLenum, GLenum, unsigned, GLenum severity,
                                  int, const char* message, const void*) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            TE_CORE_CRITICAL(message);
            return;
        case GL_DEBUG_SEVERITY_MEDIUM:
            TE_CORE_ERROR(message);
            return;
        case GL_DEBUG_SEVERITY_LOW:
            TE_CORE_WARN(message);
            return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            TE_CORE_TRACE(message);
            return;
        default:
            TE_CORE_ASSERT(false, "Unknown severity level!");
    }
}
#endif

void GLContext::init(glbinding::GetProcAddress procAddr) {
    glbinding::initialize(procAddr);

    glEnable(GL_DEPTH_TEST);
#ifdef DEBUG_BUILD
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(OpenGLMessageCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                          GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif

    // Blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // MSAA
    glEnable(GL_MULTISAMPLE);
}

}  // namespace te
