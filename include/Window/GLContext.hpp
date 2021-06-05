#ifndef GL_CONTEXT_HPP
#define GL_CONTEXT_HPP

#include <string>

class GLFWwindow;

extern "C" GLFWwindow* glfwGetCurrentContext();

namespace te {

class GLContext {
   protected:
    GLContext(int width, int height, const std::string& title, int samples);

    virtual ~GLContext() = default;

   private:
    static void errorCallback(int error, const char* description);

    static void framebufferSizeCB(GLFWwindow* window, int width, int height);

    static void keyCallback(GLFWwindow* window, int key, int scanCode,
                            int action, int mod);

    static void mouseMovementCallback(GLFWwindow* window, double x, double y);

    static void mouseButtonCallback(GLFWwindow* window, int button, int type,
                                    int mod);

    static void mouseWheelCallback(GLFWwindow* window, double xOffset,
                                   double yOffset);
};

}  // namespace te

#endif
