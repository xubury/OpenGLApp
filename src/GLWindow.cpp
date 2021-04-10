#include <iostream>

#include "GLWindow.hpp"

void GLWindow::framebufferSizeCB(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void GLWindow::errorCallback(int error, const char* description) {
    std::cerr << "Error: " << description << ", Code: " << error << std::endl;
}

GLWindow::GLWindow() : m_window(nullptr), m_initilaized(false) { initialize(); }

GLWindow::~GLWindow() {
    if (m_initilaized) close();
}

bool GLWindow::initialize() {
    if (m_initilaized) return true;

    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
        std::cout << "Failed to initialize glfw." << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (m_window == NULL) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCB);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return false;
    }

    m_initilaized = true;
    return true;
}

void GLWindow::processInput() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
}

void GLWindow::pollEvents() {
    glfwPollEvents();
    processInput();
}

void GLWindow::update() {}

void GLWindow::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_window);
}

bool GLWindow::shouldClose() { return glfwWindowShouldClose(m_window); }

void GLWindow::run() {
    if (!m_initilaized) {
        return;
    }
    while (!shouldClose()) {
        pollEvents();
        update();
        render();
    }
    close();
}

void GLWindow::close() { glfwTerminate(); }
