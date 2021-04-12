#ifndef GL_CONTEXT_HPP
#define GL_CONTEXT_HPP

#include <string>

class GLFWwindow;
class GlContext {
   public:
    bool shouldClose();

    void setShouldClose(bool close = true);

    void swapBuffers();

    void close();

    static GLFWwindow* context;

   protected:
    GlContext(int width, int height, const std::string& title);

    ~GlContext();

   private:
    static void errorCallback(int error, const char* description);

    static void framebufferSizeCB(GLFWwindow* window, int width, int height);

    static void keyCallback(GLFWwindow* window, int key, int scanCode,
                            int action, int mods);

    static void mouseMovementCallback(GLFWwindow* window, double x, double y);
};

#endif
