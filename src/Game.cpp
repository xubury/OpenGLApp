#include <Game.hpp>
#include <Component/BoundingBox.hpp>
#include <Component/Transform.hpp>
#include <Entity/Camera.hpp>
#include <Entity/ModelEntity.hpp>
#include <Entity/Cube.hpp>
#include <Entity/Sphere.hpp>
#include <Entity/Light.hpp>
#include <Editor/Editor.hpp>

#include <iostream>

void Game::addSphere(const glm::vec3& pos, const TextureArray& textures) {
    int id = m_app.entities.create<Sphere>();
    EntityBase* sphere = m_app.entities.getPtr(id);
    sphere->setTextures(textures);
    sphere->component<Transform>()->setPosition(pos);
    sphere->setName(typeid(*sphere).name());
}

void Game::addCube(const glm::vec3& pos, float width, float height,
                   float length, const TextureArray& textures) {
    int id = m_app.entities.create<Cube>(width, height, length);
    EntityBase* cube = m_app.entities.getPtr(id);
    cube->setTextures(textures);
    cube->component<Transform>()->setPosition(pos);
    cube->setName(typeid(*cube).name());
}

void Game::addModel(const std::string& path) {
    int id = m_app.entities.create<ModelEntity>();
    ModelEntity* model = m_app.entities.getPtr<ModelEntity>(id);
    model->load(path);
    model->setName(typeid(*model).name());
}

Game::Game(const Settings& settings)
    : m_window(settings.width, settings.height, settings.title),
      m_activeCam(0),
      m_editorMode(settings.editor) {
    m_activeCam = m_cameras.create<Camera>(0, 0, settings.width,
                                           settings.height, glm::vec3(0, 3, 3));
    m_cameras.get(m_activeCam)
        .component<Transform>()
        ->setEulerAngle(glm::vec3(glm::radians(-30.f), glm::radians(-2.f), 0));
    m_cameras.get(m_activeCam).setPosition(glm::vec3(-2, 20, 25));

    m_light = m_app.entities.create<Light>();
    m_app.entities.getPtr<Light>(m_light)->amibent = glm::vec3(0.5f);
    m_app.entities.getPtr<Light>(m_light)->diffuse = glm::vec3(0.5f);
    m_app.entities.getPtr<Light>(m_light)->specular = glm::vec3(0.5f);
    m_app.entities.getPtr<Light>(m_light)->setPosition(glm::vec3(0, 5, -3));
    m_app.entities.getPtr<Light>(m_light)
        ->component<Transform>()
        ->setEulerAngle(glm::vec3(glm::radians(45.f), glm::radians(45.f), 0));

    m_app.systems.add<BoundingBoxSystem>();
    m_app.systems.add<TransformSystem>();
    m_shader.load("shader/vertex.glsl", "shader/fragment.glsl");

    // m_shader.use();
    // m_shader.setVec3("pointLight.position", glm::vec3(0.0f, 0.0f, 2.0f));
    // m_shader.setVec3("pointLight.direction", glm::vec3(0.0f, 0.0f, -1.0f));
    // m_shader.setVec3("pointLight.ambient", glm::vec3(0.5f));
    // m_shader.setVec3("pointLight.diffuse", glm::vec3(0.5f));
    // m_shader.setVec3("pointLight.specular", glm::vec3(1.0f));
    // m_shader.setFloat("pointLight.constant", 1.0f);
    // m_shader.setFloat("pointLight.linear", 0.09f);
    // m_shader.setFloat("pointLight.quadratic", 0.032f);
    // m_shader.setFloat("pointLight.cutOff", glm::cos(glm::radians(12.5f)));
    // m_shader.setFloat("pointLight.outerCutOff",
    // glm::cos(glm::radians(15.5f)));

    // glm::vec3 positions[] = {
    //     glm::vec3(-2.0f, 0.0f, 0.0f),   glm::vec3(2.0f, 5.0f, -15.0f),
    //     glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    //     glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    //     glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    //     glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
    TextureArray textures;
    textures.loadFromValue(glm::vec3(0.5f), Texture::AMBIENT);
    textures.loadFromValue(glm::vec3(1.f), Texture::DIFFUSE);
    textures.loadFromValue(glm::vec3(1.f), Texture::SPECULAR);
    // addModel("resources/models/backpack/backpack.obj");

    // for (int i = 0; i < 10; ++i) {
    //     glm::vec3 offset(0);
    //     offset[i % 3] = i % 5;
    //     addCube(positions[i] + offset, textures);
    //     addSphere(positions[i], textures);
    // }
    //
    addCube(glm::vec3(0), 10, 0.2, 10, textures);
    addCube(glm::vec3(2, 3, 3), 1, 1, 1, textures);
    addSphere(glm::vec3(0, 3, 0), textures);

    m_window.setFramerateLimit(settings.frameRateLimit);

    m_frameBuffer.initialize(settings.width, settings.height, settings.samples);
}

void Game::update(Time& deltaTime) {
    // auto end = m_app.entities.end();
    // for (auto cur = m_app.entities.begin(); cur != end; ++cur) {
    //     m_app.entities.get(*cur).component<Transform>()->rotateLocal(
    //         glm::radians(0.01f) * deltaTime.as<MilliSeconds>().count(),
    //         glm::vec3(1, 2, 3));
    // }
    m_app.update(deltaTime);
}

void Game::render() {
    RenderStates states;
    states.light = m_app.entities.getPtr<LightBase>(m_light);
    states.camera = m_cameras.getPtr<CameraBase>(m_activeCam);

    auto end = m_app.entities.end();

    // draw depth map
    m_frameBuffer.setupDepthDraw();
    states.depthMapDraw = true;
    states.shader = &m_frameBuffer.s_shadowShader;
    states.depthMapTexture = m_frameBuffer.getDepthMapTexture();
    for (auto cur = m_app.entities.begin(); cur != end; ++cur) {
        states.transform =
            m_app.entities.get(*cur).component<Transform>()->getMatrix();
        m_window.draw(m_app.entities.get(*cur), states);
    }

    // normal draw
    m_frameBuffer.activate();
    m_window.clear();
    states.depthMapDraw = false;
    states.shader = &m_shader;
    states.depthMapTexture = m_frameBuffer.getDepthMapTexture();
    for (auto cur = m_app.entities.begin(); cur != end; ++cur) {
        states.transform =
            m_app.entities.get(*cur).component<Transform>()->getMatrix();
        states.textures = m_app.entities.get(*cur).getTextures();
        m_window.draw(m_app.entities.get(*cur), states);
    }

    if (m_editorMode) {
        Editor::instance().render();
    } else {
        m_frameBuffer.draw();
    }

    m_window.display();
}

void Game::run(int minFps) {
    Clock clock;
    Time timeSinceLastUpdate = Time::Zero;
    Time timePerFrame = seconds(1.0 / minFps);
    Camera* camera = m_cameras.getPtr<Camera>(m_activeCam);

    if (m_editorMode) {
        Editor::instance().context.setFrameBuffer(&m_frameBuffer);
        Editor::instance().context.setWindow(&m_window);
        Editor::instance().context.setEntityManager(&m_app.entities);
        Editor::instance().context.setCamrea(camera);
    }
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
                // When minimized， the width and height will drop to zero
                if (event.size.width > 0 && event.size.height > 0) {
                    m_frameBuffer.update(event.size.width, event.size.height);
                    camera->setViewportSize(event.size.width,
                                            event.size.height);
                }
            }
            camera->processEvent(event);
        }
        camera->processEvents();
        timeSinceLastUpdate = clock.restart();
        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            update(timePerFrame);
        }
        update(timeSinceLastUpdate);
        render();
    }
    Editor::instance().close();
    m_window.close();
}
