#include <Application.hpp>

#include <iostream>

Application::Application(int width, int height, const std::string& title)
    : m_window(width, height, title) {
    m_shader.loadFromFile("shader/vertex.glsl", "shader/fragment.glsl");
    DebugShader::debugShader.loadFromFile("shader/debugVertex.glsl",
                                          "shader/debugFragment.glsl");

    m_textureManager["awesomeface"].loadFromFile(
        "resources/textures/awesomeface.png", Texture::TextureType::DIFFUSE);
    m_textureManager["container"].loadFromFile(
        "resources/textures/container2.png", Texture::TextureType::DIFFUSE);
    m_textureManager["container"].loadFromFile(
        "resources/textures/container2_specular.png",
        Texture::TextureType::SPECULAR);

    m_shader.use();
    m_shader.setVec3("light.position", glm::vec3(0.0f, 0.0f, 3.0f));
    m_shader.setVec4("light.direction", glm::vec4(0.0f, 0.0f, -1.0f, 1.f));
    m_shader.setVec3("light.ambient", glm::vec3(0.5f));
    m_shader.setVec3("light.diffuse", glm::vec3(0.5f));
    m_shader.setVec3("light.specular", glm::vec3(1.0f));
    m_shader.setFloat("light.constant", 1.0f);
    m_shader.setFloat("light.linear", 0.09f);
    m_shader.setFloat("light.quadratic", 0.032f);
    m_shader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    m_shader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
    for (int i = 0; i < 10; ++i) {
        m_cube[i].translate(cubePositions[i]);
    }
    m_window.setCamera<ControlCamera>(0, 0, width, height,
                                      glm::vec3(0.f, 0.f, 3.f));
}

void Application::update() {
    for (int i = 0; i < 10; ++i) {
        m_cube[i].rotate(1.f, glm::vec3(1.0f, 0.3f, 0.5f));
        m_cube[i].m_aabb.update(m_cube[i].getTransform());
    }
}

void Application::render() {
    m_window.clear();

    RenderStates states;
    m_shader.use();
    states.shader = &m_shader;
    states.textures = &m_textureManager.at("container");

    for (int i = 0; i < 10; ++i) {
        m_cube[i].draw(m_window, states);
    }

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
