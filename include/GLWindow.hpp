#ifndef GL_WINDOW_HPP
#define GL_WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"

class RenderWindow {
   public:
    RenderWindow();
    ~RenderWindow();
    void run();

   private:
    bool initialize();
    void update();
    void render();
    void processEvents();
    bool shouldClose();
    void processInput();

    void close();

    static void errorCallback(int error, const char* description);
    static void framebufferSizeCB(GLFWwindow* window, int width, int height);

    GLFWwindow* m_window;
    bool m_initilaized;

    uint32_t m_VBO;
    uint32_t m_VAO;
    uint32_t m_EBO;

    Shader m_shader;
};

#endif
