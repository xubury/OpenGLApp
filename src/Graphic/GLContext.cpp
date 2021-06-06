#include "Graphic/GLContext.hpp"
#include "Graphic/OpenGL.hpp"
#include "Core/Assert.hpp"

namespace te {

static void glDebugMessage(GLenum, GLenum, GLuint, GLenum severity, GLsizei,
                           const GLchar *message, const void *) {
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            TE_CORE_CRITICAL("OpenGL Debug HIGH: {0}", message);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            TE_CORE_CRITICAL("OpenGL Debug Medium: {0}", message);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            TE_CORE_CRITICAL("OpenGL Debug Low: {0}", message);
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            break;
        default:
            break;
    }
}

void GLContext::init(glbinding::GetProcAddress procAddr) {
    glbinding::initialize(procAddr);

    // Debug Message
    glDebugMessageCallback(&glDebugMessage, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glEnable(GL_DEPTH_TEST);

    // Blend
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // MSAA
    glEnable(GL_MULTISAMPLE);
}

}  // namespace te
