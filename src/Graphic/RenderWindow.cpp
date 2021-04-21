#include <Graphic/RenderWindow.hpp>
#include <Window/Event.hpp>

#include <iostream>
#include <thread>

RenderWindow::RenderWindow(int width, int height, const std::string& title)
    : GlContext(width, height, title), m_framerateLimit(MicroSeconds(0)) {}

RenderWindow::~RenderWindow() { close(); }

void RenderWindow::display() {
    glfwSwapBuffers(glfwGetCurrentContext());
    if (m_framerateLimit != Time(MicroSeconds(0))) {
        std::this_thread::sleep_for(m_framerateLimit -
                                    m_clock.getElapsedTime());
        m_clock.restart();
    }
}

bool RenderWindow::shouldClose() {
    return glfwWindowShouldClose(glfwGetCurrentContext());
}

void RenderWindow::setShouldClose(bool close) {
    glfwSetWindowShouldClose(glfwGetCurrentContext(), close);
}

void RenderWindow::close() { glfwTerminate(); }

void RenderWindow::setFramerateLimit(uint32_t fps) {
    // FIXME: this is not correct!
    m_framerateLimit = Time(Time::as<Seconds>(1.0 / fps));
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
