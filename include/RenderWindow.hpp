#ifndef RENDER_WINDOW_HPP
#define RENDER_WINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "RenderTarget.hpp"

class RenderWindow : public RenderTarget {
   public:
    RenderWindow(int width, int height, const std::string& title);
    ~RenderWindow();

    void initialize(int width, int height, const std::string& title);

    void clear();
    void swapBuffers();

    void processEvents();
    bool shouldClose();
    void processInput();

    void close();

   private:
    static void errorCallback(int error, const char* description);
    static void framebufferSizeCB(GLFWwindow* window, int width, int height);

    GLFWwindow* m_window;
    static bool m_initilaized;
};

#endif
