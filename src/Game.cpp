#include <Game.hpp>
#include <Entity/Model.hpp>
#include <Entity/Cube.hpp>
#include <Component/BoundingBox.hpp>
#include <Component/Transform.hpp>
#include <iostream>

void Game::addCube(const glm::vec3& pos, const TextureArray& textures) {
    int id = m_app.entities.create<Cube>();
    Cube* cube = dynamic_cast<Cube*>(m_app.entities.getPtr(id));
    cube->setTextures(textures);
    cube->component<Transform>()->setPosition(pos);
}

void Game::addModel(const std::string& path) {
    int id = m_app.entities.create<Model>();
    Model* model = dynamic_cast<Model*>(m_app.entities.getPtr(id));
    model->loadFromFile(path);
}

Game::Game(int width, int height, const std::string& title)
    : m_window(width, height, title) {
    m_app.systems.add<BoundingBoxSystem>();
    m_app.systems.add<TransformSystem>();
    m_shader.loadFromFile("shader/vertex.glsl", "shader/fragment.glsl");

    m_shader.use();
    m_shader.setVec3("pointLight.position", glm::vec3(0.0f, 0.0f, 2.0f));
    m_shader.setVec3("pointLight.direction", glm::vec3(0.0f, 0.0f, -1.0f));
    m_shader.setVec3("pointLight.ambient", glm::vec3(0.5f));
    m_shader.setVec3("pointLight.diffuse", glm::vec3(0.5f));
    m_shader.setVec3("pointLight.specular", glm::vec3(1.0f));
    m_shader.setFloat("pointLight.constant", 1.0f);
    m_shader.setFloat("pointLight.linear", 0.09f);
    m_shader.setFloat("pointLight.quadratic", 0.032f);
    m_shader.setFloat("pointLight.cutOff", glm::cos(glm::radians(12.5f)));
    m_shader.setFloat("pointLight.outerCutOff", glm::cos(glm::radians(15.5f)));

    glm::vec3 cubePositions[] = {
        glm::vec3(-2.0f, 0.0f, 0.0f),   glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
    TextureArray containerTextures;
    containerTextures.loadFromFile("resources/textures/container2.png",
                                   Texture::DIFFUSE);
    containerTextures.loadFromFile("resources/textures/container2_specular.png",
                                   Texture::SPECULAR);
    for (int i = 0; i < 10; ++i) {
        addCube(cubePositions[i], containerTextures);
    }
    addModel("resources/models/backpack/backpack.obj");

    m_window.setCamera<ControlCamera>(0, 0, width, height,
                                      glm::vec3(0.f, 0.f, 3.f));
    m_window.setFramerateLimit(120);

    m_frameBuffer.create(width, height);
}

void Game::update(Time& deltaTime) {
    m_app.update(deltaTime);

    auto end = m_app.entities.end();
    for (auto cur = m_app.entities.begin(); cur != end; ++cur) {
        m_app.entities.get(*cur).component<Transform>()->rotate(
            glm::radians(1.0f) * deltaTime.as<MilliSeconds>().count(),
            glm::vec3(1.0, 2.0f, 3.0f));
    }
}

void Game::render() {
    m_frameBuffer.use();
    m_window.clear();

    RenderStates states;
    states.shader = &m_shader;
    auto end = m_app.entities.end();
    for (auto cur = m_app.entities.begin(); cur != end; ++cur) {
        m_window.draw(m_app.entities.get(*cur), states);
    }

    m_app.systems.system<BoundingBoxSystem>()->draw(m_app.entities, m_window);
    m_app.systems.system<TransformSystem>()->draw(m_app.entities, m_window);

    m_frameBuffer.draw();
    m_window.display();
}

void Game::run(int minFps) {
    Clock clock;
    Time timeSinceLastUpdate = Time::Zero;
    Time timePerFrame = seconds(1.0 / minFps);

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
        timeSinceLastUpdate = clock.restart();
        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            update(timePerFrame);
        }
        update(timeSinceLastUpdate);
        render();
    }
    m_window.close();
}
