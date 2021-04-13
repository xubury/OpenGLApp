#ifndef GL_CONTEXT_HPP
#define GL_CONTEXT_HPP

#include <string>

class GLFWwindow;

class GlContext {
   protected:
    GlContext(int width, int height, const std::string& title);

    virtual ~GlContext() = default;

    static GLFWwindow* m_context;

   private:
    static void errorCallback(int error, const char* description);

    static void framebufferSizeCB(GLFWwindow* window, int width, int height);

    static void keyCallback(GLFWwindow* window, int key, int scanCode,
                            int action, int mods);

    static void mouseMovementCallback(GLFWwindow* window, double x, double y);
};

#endif
