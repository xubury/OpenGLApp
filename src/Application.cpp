#include <Application.hpp>

#include <iostream>

Application::Application(int width, int height, const std::string& title)
    : m_window(width, height, title) {
    m_shader.loadFromFile("shader/vertex.glsl", "shader/fragment.glsl");
    DebugShader::debugShader.loadFromFile("shader/debug.vs", "shader/debug.fs");

    m_textureManager["awesomeface"].loadFromFile(
        "resources/textures/awesomeface.png", Texture::TextureType::DIFFUSE);
    m_textureManager["container"].loadFromFile(
        "resources/textures/container2.png", Texture::TextureType::DIFFUSE);
    m_textureManager["container"].loadFromFile(
        "resources/textures/container2_specular.png",
        Texture::TextureType::SPECULAR);

    m_shader.use();
    m_shader.setVec3("light.position", glm::vec3(0.0f, 0.0f, 1.0f));
    m_shader.setVec3("light.ambient", glm::vec3(0.5f));
    m_shader.setVec3("light.diffuse", glm::vec3(0.5f));
    m_shader.setVec3("light.specular", glm::vec3(1.0f));

    m_cube2.translate(glm::vec3(1.0, 0.f, 0.f));
    m_window.setCamera<ControlCamera>(0, 0, width, height,
                                      glm::vec3(0.f, 0.f, 3.f));
}

void Application::update() {
    m_cube1.rotate(1.f, glm::vec3(1.0f, 0.3f, 0.5f));
    glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(1.f),
                                   glm::vec3(0.0f, 0.0f, 1.0f));
    m_cube2.transform(rotate);
    m_cube1.m_aabb.update(m_cube1.getTransform());
    m_cube2.m_aabb.update(m_cube2.getTransform());
}

void Application::render() {
    m_window.clear();

    RenderStates states;
    m_shader.use();
    states.shader = &m_shader;
    states.textures = &m_textureManager.at("container");
    m_cube1.draw(m_window, states);

    states.textures = &m_textureManager.at("awesomeface");
    m_cube2.draw(m_window, states);

    m_window.swapBuffers();
}

void Application::run() {
    while (!m_window.shouldClose()) {
        Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == Event::KEY_PRESSED) {
                switch (event.key.code) {
                    case Keyboard::ESCAPE:
                        m_window.setShouldClose();
                        break;
                    default:
                        break;
                }
            } else if (event.type == Event::EventType::RESIZED) {
                m_window.getCamera().setSize(event.size.width,
                                             event.size.height);
            }
            m_window.processEvent(event);
        }
        m_window.processEvents();
        update();
        render();
    }
    m_window.close();
}
