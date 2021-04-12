#include <iostream>
#include "RenderWindow.hpp"

bool RenderWindow::m_initilaized = false;

void RenderWindow::framebufferSizeCB(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void RenderWindow::errorCallback(int error, const char* description) {
    std::cerr << "Error: " << description << ", Code: " << error << std::endl;
}

void RenderWindow::initialize(int width, int height, const std::string& title) {
    if (!m_initilaized) {
        glfwSetErrorCallback(errorCallback);
        if (!glfwInit()) {
            std::cout << "Failed to initialize glfw." << std::endl;
            return;
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    } else {
        close();
    }

    m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (m_window == NULL) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCB);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return;
    }
    glEnable(GL_DEPTH_TEST);

    create();
    m_initilaized = true;
    return;
}

RenderWindow::RenderWindow(int width, int height, const std::string& title) {
    initialize(width, height, title);
}

RenderWindow::~RenderWindow() { close(); }

// FIXME: process input outside the window
void RenderWindow::processInput() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        getCamera().move(Camera::Movement::FORWARD, 0.1f);
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        getCamera().move(Camera::Movement::BACKWRAD, 0.1f);
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        getCamera().move(Camera::Movement::LEFT, 0.1f);
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        getCamera().move(Camera::Movement::RIGHT, 0.1f);
}

void RenderWindow::processEvents() {
    glfwPollEvents();
    processInput();
}

void RenderWindow::swapBuffers() { glfwSwapBuffers(m_window); }

bool RenderWindow::shouldClose() { return glfwWindowShouldClose(m_window); }

void RenderWindow::close() { glfwTerminate(); }
