#include "Application.hpp"

#include <iostream>

Application::Application(int width, int height, const std::string& title)
    : m_window(width, height, title) {
    m_shader.load("shader/vertex.glsl", "shader/fragment.glsl");
    m_textureManager.load("awesomeface", "resources/textures/awesomeface.png",
                          GL_RGBA);
    m_textureManager.load("container", "resources/textures/container.jpg",
                          GL_RGB);

    m_shader.setInt("texture0", 0);
    m_shader.setInt("texture1", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureManager.get("container").id);
}

void Application::update() {}

void Application::render() {
    m_window.clear();
    m_shader.use();
    m_cube.draw(m_window);
    m_window.swapBuffers();
}

void Application::run() {
    while (!m_window.shouldClose()) {
        m_window.processEvents();
        update();
        render();
    }
    m_window.close();
}
