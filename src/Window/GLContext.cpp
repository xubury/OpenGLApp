#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#include <Window/GLContext.hpp>
#include <Graphic/Shader.hpp>
#include <Window/Event.hpp>
#include <Window/RenderWindow.hpp>

#include <GLFW/glfw3.h>
#include <iostream>

void GLContext::framebufferSizeCB(GLFWwindow* window, int width, int height) {
    Event event;
    event.type = Event::EventType::RESIZED;
    event.size.width = width;
    event.size.height = height;
    auto win = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
    if (win) win->pushEvent(event);
}

void GLContext::errorCallback(int error, const char* description) {
    std::cerr << "Error: " << description << ", Code: " << error << std::endl;
}

void GLContext::keyCallback(GLFWwindow* window, int key, int, int action,
                            int mod) {
    Event event;
    if (action == GLFW_RELEASE) {
        event.type = Event::EventType::KEY_RELEASED;
    } else if (action == GLFW_PRESS) {
        event.type = Event::EventType::KEY_PRESSED;
    }
    event.key.code = (Keyboard::Key)key;

    switch (mod) {
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

void GLContext::mouseMovementCallback(GLFWwindow* window, double x, double y) {
    Event event;
    event.type = Event::EventType::MOUSE_MOVED;
    event.mouseMove.x = x;
    event.mouseMove.y = y;
    auto win = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
    if (win) win->pushEvent(event);
}

void GLContext::mouseButtonCallback(GLFWwindow* window, int button, int type,
                                    int mod) {
    Event event;
    event.type = type == GLFW_PRESS ? Event::EventType::MOUSE_BUTTON_PRESSED
                                    : Event::EventType::MOUSE_BUTTON_RELEASED;
    event.mouseButton.button = (Mouse::Button)button;
    switch (mod) {
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

void GLContext::mouseWheelCallback(GLFWwindow* window, double xOffset,
                                   double yOffset) {
    Event event;
    event.type = Event::MOUSE_WHEEL_SCROLLED;
    event.mouseWheel.xOffset = xOffset;
    event.mouseWheel.yOffset = yOffset;

    glfwGetCursorPos(glfwGetCurrentContext(), &event.mouseWheel.x,
                     &event.mouseWheel.y);
    auto win = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
    if (win) win->pushEvent(event);
}

GLContext::GLContext(int width, int height, const std::string& title) {
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
        std::cout << "Failed to initialize glfw." << std::endl;
        exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // MSAA
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* win =
        glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (win == NULL) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(win);
    glbinding::initialize(glfwGetProcAddress);

    // setup callbacks
    glfwSetWindowUserPointer(win, this);
    glfwSetFramebufferSizeCallback(win, framebufferSizeCB);
    glfwSetKeyCallback(win, keyCallback);
    glfwSetCursorPosCallback(win, mouseMovementCallback);
    glfwSetMouseButtonCallback(win, mouseButtonCallback);
    glfwSetScrollCallback(win, mouseWheelCallback);
    // glfwSetInputMode(m_context, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    using namespace gl;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // MSAA
    glEnable(GL_MULTISAMPLE);
}
