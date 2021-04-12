#include "Application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Keyboard.hpp"
#include "RenderStates.hpp"

Application::Application(int width, int height, const std::string& title)
    : m_window(width, height, title),
      m_camera(width, height, glm::vec3(0.f, 0.f, 3.f)),
      m_isFirstMouse(true) {
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

    RenderStates states;
    states.setShader(m_shader);
    states.setTexture(m_textureManager.get("container"));
    states.setCamera(m_camera);
    m_cube1.draw(m_window, states);

    states.setTexture(m_textureManager.get("awesomeface"));
    m_cube2.draw(m_window, states);

    m_window.swapBuffers();
}

void Application::run() {
    while (!m_window.shouldClose()) {
        Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == Event::KeyPressed) {
                switch (event.key.code) {
                    case Keybaord::W:
                        m_camera.move(Camera::Movement::FORWARD, 0.1f);
                        break;
                    case Keybaord::S:
                        m_camera.move(Camera::Movement::BACKWRAD, 0.1f);
                        break;
                    case Keybaord::A:
                        m_camera.move(Camera::Movement::LEFT, 0.1f);
                        break;
                    case Keybaord::D:
                        m_camera.move(Camera::Movement::RIGHT, 0.1f);
                        break;
                    case Keybaord::ESCAPE:
                        m_window.setShouldClose();
                        break;
                }
            } else if (event.type == Event::MouseMoved) {
                glm::vec2 currentMousePos =
                    glm::vec2(event.mouseMove.x, event.mouseMove.y);
                if (m_isFirstMouse) {
                    m_isFirstMouse = false;
                } else {
                    glm::vec2 offset = currentMousePos - m_lastMousePos;
                    m_camera.rotate(offset.x * MOUSE_SENSITIVITY,
                                    -offset.y * MOUSE_SENSITIVITY);
                }
                m_lastMousePos = currentMousePos;
            }
        }
        m_window.processEvents();
        update();
        render();
    }
    m_window.close();
}
