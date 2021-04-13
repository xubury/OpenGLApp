#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GlContext.hpp"
#include "Event.hpp"
#include "RenderWindow.hpp"

GLFWwindow* GlContext::m_context;

void GlContext::framebufferSizeCB(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void GlContext::errorCallback(int error, const char* description) {
    std::cerr << "Error: " << description << ", Code: " << error << std::endl;
}

void GlContext::keyCallback(GLFWwindow* window, int key, int, int action,
                            int mods) {
    Event event;
    event.key.code = key;
    if (action == GLFW_RELEASE) {
        event.type = Event::KeyReleased;
    } else if (action == GLFW_PRESS) {
        event.type = Event::KeyPressed;
    }

    switch (mods) {
        case GLFW_MOD_SHIFT:
            event.key.shift = true;
            break;
        case GLFW_MOD_CONTROL:
            event.key.control = true;
            break;
        case GLFW_MOD_ALT:
            event.key.alt = true;
            break;
        case GLFW_MOD_SUPER:
            event.key.system = true;
        default:
            break;
    }
    auto win = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
    if (win) win->pushEvent(event);
}

void GlContext::mouseMovementCallback(GLFWwindow* window, double x, double y) {
    Event event;
    event.type = Event::MouseMoved;
    event.mouseMove.x = x;
    event.mouseMove.y = y;
    auto win = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
    if (win) win->pushEvent(event);
}

GLFWwindow* GlContext::context() { return m_context; }

GlContext::GlContext(int width, int height, const std::string& title) {
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
        std::cout << "Failed to initialize glfw." << std::endl;
        exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_context = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (m_context == NULL) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(m_context);

    glfwSetWindowUserPointer(m_context, this);

    glfwSetFramebufferSizeCallback(m_context, framebufferSizeCB);
    glfwSetKeyCallback(m_context, keyCallback);
    glfwSetCursorPosCallback(m_context, mouseMovementCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        exit(-1);
    }
    glEnable(GL_DEPTH_TEST);
}

GlContext::~GlContext() { close(); }

void GlContext::swapBuffers() { glfwSwapBuffers(m_context); }

bool GlContext::shouldClose() { return glfwWindowShouldClose(m_context); }

void GlContext::setShouldClose(bool close) {
    glfwSetWindowShouldClose(m_context, close);
}

void GlContext::close() { glfwTerminate(); }
