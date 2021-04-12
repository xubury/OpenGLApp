#include <iostream>
#include "RenderWindow.hpp"
#include "Event.hpp"

void RenderWindow::framebufferSizeCB(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void RenderWindow::errorCallback(int error, const char* description) {
    std::cerr << "Error: " << description << ", Code: " << error << std::endl;
}

void RenderWindow::keyCallback(GLFWwindow* window, int key, int, int action,
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
        default:
            break;
    }
    auto win = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
    if (win) win->pushEvent(event);
}

void RenderWindow::mouseMovementCallback(GLFWwindow* window, double x,
                                         double y) {
    Event event;
    event.type = Event::MouseMoved;
    event.mouseMove.x = x;
    event.mouseMove.y = y;
    auto win = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
    if (win) win->pushEvent(event);
}

void RenderWindow::initialize(int width, int height, const std::string& title) {
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
        std::cout << "Failed to initialize glfw." << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (m_window == NULL) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(m_window);

    glfwSetWindowUserPointer(m_window, this);

    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCB);
    glfwSetKeyCallback(m_window, keyCallback);
    glfwSetCursorPosCallback(m_window, mouseMovementCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return;
    }
    glEnable(GL_DEPTH_TEST);

    create();
    return;
}

RenderWindow::RenderWindow(int width, int height, const std::string& title) {
    initialize(width, height, title);
}

RenderWindow::~RenderWindow() { close(); }

void RenderWindow::processEvents() { glfwPollEvents(); }

void RenderWindow::swapBuffers() { glfwSwapBuffers(m_window); }

bool RenderWindow::shouldClose() { return glfwWindowShouldClose(m_window); }

void RenderWindow::setShouldClose(bool close) {
    glfwSetWindowShouldClose(m_window, close);
}

void RenderWindow::close() { glfwTerminate(); }

bool RenderWindow::pollEvent(Event& event) { return popEvent(event, false); }

void RenderWindow::pushEvent(const Event& event) { m_events.push(event); }

bool RenderWindow::popEvent(Event& event, bool block) {
    if (m_events.empty()) {
        processEvents();
        if (block) {
            // TODO: block mode
        }
    }
    if (m_events.size()) {
        event = m_events.front();
        m_events.pop();
        return true;
    }
    return false;
}
