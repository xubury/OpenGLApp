#include <Graphic/RenderWindow.hpp>
#include <Window/Event.hpp>

#include <thread>
#include <iostream>

RenderWindow::RenderWindow(int width, int height, const std::string& title)
    : GlContext(width, height, title) {}

RenderWindow::~RenderWindow() { close(); }

void RenderWindow::display() {
    glfwSwapBuffers(glfwGetCurrentContext());
    if (m_framerateLimit != Time::Zero) {
        std::this_thread::sleep_for(m_framerateLimit -
                                    m_clock.getElapsedTime());
        m_frameRate =
            1.f / m_clock.getElapsedTime().as<MilliSeconds>().count() * 1000;
        m_clock.restart();
    }
}

float RenderWindow::getFrameRate() const {
    return m_frameRate;
}

bool RenderWindow::shouldClose() {
    return glfwWindowShouldClose(glfwGetCurrentContext());
}

void RenderWindow::setShouldClose(bool close) {
    glfwSetWindowShouldClose(glfwGetCurrentContext(), close);
}

void RenderWindow::close() { glfwTerminate(); }

void RenderWindow::setFramerateLimit(uint32_t fps) {
    if (fps != 0)
        m_framerateLimit = seconds(1.0 / fps);
    else
        m_framerateLimit = Time::Zero;
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
