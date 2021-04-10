#ifndef GL_WINDOW_HPP
#define GL_WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLWindow {
   public:
    GLWindow();
    ~GLWindow();
    void run();

   private:
    bool initialize();
    void update();
    void render();
    void pollEvents();
    bool shouldClose();
    void processInput();

    void close();

    static void errorCallback(int error, const char* description);
    static void framebufferSizeCB(GLFWwindow* window, int width, int height);

    GLFWwindow* m_window;
    bool m_initilaized;
};

#endif
