#include <iostream>
#include "RenderWindow.hpp"
#include "Event.hpp"

RenderWindow::RenderWindow(int width, int height, const std::string& title)
    : GlContext(width, height, title) {}

RenderWindow::~RenderWindow() { close(); }

void RenderWindow::swapBuffers() { glfwSwapBuffers(glfwGetCurrentContext()); }

bool RenderWindow::shouldClose() {
    return glfwWindowShouldClose(glfwGetCurrentContext());
}

void RenderWindow::setShouldClose(bool close) {
    glfwSetWindowShouldClose(glfwGetCurrentContext(), close);
}

void RenderWindow::close() { glfwTerminate(); }

void RenderWindow::processEvents() { glfwPollEvents(); }

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
