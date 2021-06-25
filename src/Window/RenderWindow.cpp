#include "Window/RenderWindow.hpp"
#include "Graphic/GLContext.hpp"
#include "Core/Assert.hpp"
#include "Window/Event.hpp"

#include <GLFW/glfw3.h>
#include <thread>
#include <iostream>

namespace te {

RenderWindow::RenderWindow(int width, int height, const std::string& title,
                           int samples) {
    TE_CORE_ASSERT(width > 0 && height > 0,
                   "RenderWindow width or height <= 0.");
    TE_CORE_ASSERT(samples > 0, "RenderWindow samples <= 0.");
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
        TE_CORE_ERROR("Failed to initialize GLFW!");
        exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // MSAA
    glfwWindowHint(GLFW_SAMPLES, samples);

    GLFWwindow* win =
        glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (win == nullptr) {
        TE_CORE_ERROR("Failed to create GLFW window!");
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(win);

    // disable vsync
    glfwSwapInterval(0);

    // setup callbacks
    glfwSetWindowUserPointer(win, this);
    glfwSetFramebufferSizeCallback(win, framebufferSizeCB);
    glfwSetKeyCallback(win, keyCallback);
    glfwSetCursorPosCallback(win, mouseMovementCallback);
    glfwSetMouseButtonCallback(win, mouseButtonCallback);
    glfwSetScrollCallback(win, mouseWheelCallback);

    GLContext::init(glfwGetProcAddress);
}

RenderWindow::~RenderWindow() { close(); }

void RenderWindow::display() {
    glfwSwapBuffers(glfwGetCurrentContext());
    if (m_secondPerFrame != Time::Zero) {
        std::this_thread::sleep_for(m_secondPerFrame -
                                    m_clock.getElapsedTime());
    }
    m_frameRate = 1.f / m_clock.getElapsedTime().count();
    m_clock.restart();
}

float RenderWindow::getFrameRate() const { return m_frameRate; }

bool RenderWindow::shouldClose() {
    return glfwWindowShouldClose(glfwGetCurrentContext());
}

void RenderWindow::setShouldClose(bool close) {
    glfwSetWindowShouldClose(glfwGetCurrentContext(), close);
}

void RenderWindow::close() { glfwTerminate(); }

void RenderWindow::setFramerateLimit(uint32_t fps) {
    if (fps != 0)
        m_secondPerFrame = seconds(1.0f / fps);
    else
        m_secondPerFrame = Time::Zero;
}

void RenderWindow::pollEvents() { glfwPollEvents(); }

bool RenderWindow::pollEvent(Event& event) { return popEvent(event, false); }

void RenderWindow::pushEvent(const Event& event) { m_events.push(event); }

bool RenderWindow::popEvent(Event& event, bool block) {
    if (m_events.empty()) {
        pollEvents();
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

::GLFWwindow* RenderWindow::getCurrentContext() {
    return glfwGetCurrentContext();
}

void RenderWindow::captureMouse(bool capture) {
    if (capture) {
        glfwSetInputMode(getCurrentContext(), GLFW_CURSOR,
                         GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(getCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void RenderWindow::framebufferSizeCB(GLFWwindow* window, int width,
                                     int height) {
    Event event;
    event.type = Event::EventType::RESIZED;
    event.size.width = width;
    event.size.height = height;
    auto win = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
    if (win) win->pushEvent(event);
}

void RenderWindow::errorCallback(int error, const char* description) {
    TE_CORE_ERROR("GLFW error: {0}, code: {1}", description, error);
}

void RenderWindow::keyCallback(GLFWwindow* window, int key, int, int action,
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

void RenderWindow::mouseMovementCallback(GLFWwindow* window, double x,
                                         double y) {
    Event event;
    event.type = Event::EventType::MOUSE_MOVED;
    event.mouseMove.x = x;
    event.mouseMove.y = y;
    auto win = static_cast<RenderWindow*>(glfwGetWindowUserPointer(window));
    if (win) win->pushEvent(event);
}

void RenderWindow::mouseButtonCallback(GLFWwindow* window, int button, int type,
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

void RenderWindow::mouseWheelCallback(GLFWwindow* window, double xOffset,
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

}  // namespace te
