#include "Application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "RenderStates.hpp"

Application::Application(int width, int height, const std::string& title)
    : m_window(width, height, title) {
    m_shader.load("shader/vertex.glsl", "shader/fragment.glsl");
    m_textureManager.load("awesomeface", "resources/textures/awesomeface.png",
                          GL_RGBA);
    m_textureManager.load("container", "resources/textures/container.jpg",
                          GL_RGB);

    m_shader.setInt("texture0", 0);
    m_shader.setInt("texture1", 1);
    m_cube2.translate(glm::vec3(1.0, 0.f, 0.f));
}

void Application::update() {
    m_cube1.rotate(1.f, glm::vec3(1.0f, 0.3f, 0.5f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(1.f),
                                   glm::vec3(0.0f, 0.0f, 1.0f));
    m_cube2.transform(rotate);
}

void Application::render() {
    m_window.clear();
    m_shader.use();
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600,
                                  0.1f, 100.0f);
    // pass transformation matrices to the shader
    m_shader.setMat4("projection", projection);

    RenderStates states;
    states.setShader(m_shader);
    states.setTexture(m_textureManager.get("container"));
    m_cube1.draw(m_window, states);

    states.setTexture(m_textureManager.get("awesomeface"));
    m_cube2.draw(m_window, states);

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
