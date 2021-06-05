#include "Game.hpp"
#include "Component/BoundingBox.hpp"
#include "Component/Transform.hpp"
#include "Component/Light.hpp"
#include "Entity/Camera.hpp"
#include "Entity/ModelEntity.hpp"
#include "Entity/Cube.hpp"
#include "Entity/Sphere.hpp"
#include "Editor/Editor.hpp"

#include <iostream>

void Game::addSphere(const glm::vec3& pos, const TextureArray& textures) {
    int id = m_app.entities.create<Sphere>();
    Ref<EntityBase> sphere = m_app.entities.get(id);
    sphere->setTextures(textures);
    sphere->setPosition(pos);
    sphere->setName("Sphere");
}

void Game::addCube(const glm::vec3& pos, float width, float height,
                   float length, const TextureArray& textures) {
    int id = m_app.entities.create<Cube>(width, height, length);
    Ref<EntityBase> cube = m_app.entities.get(id);
    cube->setTextures(textures);
    cube->setPosition(pos);
    cube->setName("Cube");
}

void Game::addModel(const std::string& path, const glm::vec3& pos) {
    int id = m_app.entities.create<ModelEntity>();
    Ref<EntityBase> model = m_app.entities.get(id);
    dynamic_cast<ModelEntity*>(model.get())->load(path);
    model->setPosition(pos);
    model->setName("model");
}

void Game::loadShaders() {
    const char* shadowVertex =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 uLightSpaceMatrix;\n"
        "uniform mat4 uModel;\n"
        "void main() {\n"
        "    gl_Position = uLightSpaceMatrix * uModel * vec4(aPos, 1.0);\n"
        "}";
    const char* shadowFragment =
        "#version 330 core\n"
        "void main() {\n"
        "}";

    m_shaders.add("Shadow");
    m_shaders.get("Shadow")->compile(shadowVertex, shadowFragment);

    const char* fbVertex =
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "layout (location = 1) in vec2 aTexCoords;\n"
        "out vec2 texCoords;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
        "    texCoords = aTexCoords;\n"
        "}";

    const char* fbFragment =
        "#version 330 core\n"
        "out vec4 fragColor;\n"
        "in vec2 texCoords;\n"
        "uniform sampler2D uScreenTexture;\n"
        "void main() {\n"
        "    fragColor = texture(uScreenTexture, texCoords);\n"
        "}";

    m_shaders.add("FrameBuffer");
    m_shaders.get("FrameBuffer")->compile(fbVertex, fbFragment);

    m_shaders.add("Main");
    m_shaders.get("Main")->load("shader/vertex.glsl", "shader/fragment.glsl");
    m_shaders.get("Main")->bind();
    m_shaders.get("Main")->setVec3("pointLight.position",
                                   glm::vec3(0.0f, 0.0f, 2.0f));
    m_shaders.get("Main")->setVec3("pointLight.direction",
                                   glm::vec3(0.0f, 0.0f, -1.0f));
    m_shaders.get("Main")->setVec3("pointLight.ambient", glm::vec3(0.5f));
    m_shaders.get("Main")->setVec3("pointLight.diffuse", glm::vec3(0.5f));
    m_shaders.get("Main")->setVec3("pointLight.specular", glm::vec3(1.0f));
    m_shaders.get("Main")->setFloat("pointLight.constant", 1.0f);
    m_shaders.get("Main")->setFloat("pointLight.linear", 0.09f);
    m_shaders.get("Main")->setFloat("pointLight.quadratic", 0.032f);
    m_shaders.get("Main")->setFloat("pointLight.cutOff",
                                    glm::cos(glm::radians(12.5f)));
    m_shaders.get("Main")->setFloat("pointLight.outerCutOff",
                                    glm::cos(glm::radians(15.5f)));
}

void Game::loadScene() {
    uint32_t lightSource = m_app.entities.create<EntityBase>();
    m_app.entities.get(lightSource)->setName("Directional Light");
    m_app.entities.get(lightSource)->setPosition(glm::vec3(0, 8, 8));
    m_app.entities.get(lightSource)
        ->setEulerAngle(glm::vec3(glm::radians(45.f), glm::radians(180.f), 0));

    m_app.entities.get(lightSource)->add<Light>();
    auto light = m_app.entities.get(lightSource)->component<Light>();
    light->amibent = glm::vec3(0.5f);
    light->diffuse = glm::vec3(0.5f);
    light->specular = glm::vec3(0.5f);

    // uint32_t lightSource2 = m_app.entities.create<EntityBase>();
    // m_app.entities.get(lightSource2).add<Light>();
    // m_app.entities.get(lightSource2).setPosition(glm::vec3(0, 8, 8));
    // m_app.entities.get(lightSource2)
    //     .setEulerAngle(glm::vec3(glm::radians(45.f), glm::radians(180.f),
    //     0));

    // auto light2 = m_app.entities.get(lightSource2).component<Light>();
    // light2->amibent = glm::vec3(0.5f);
    // light2->diffuse = glm::vec3(0.5f);
    // light2->specular = glm::vec3(0.5f);

    // ground
    TextureArray groundTextures;
    groundTextures.loadFromValue(glm::vec3(0.7f), Texture::AMBIENT);
    groundTextures.loadFromValue(glm::vec3(0.7f), Texture::DIFFUSE);
    groundTextures.loadFromValue(glm::vec3(0.f), Texture::SPECULAR);
    addCube(glm::vec3(0), 50, 1, 50, groundTextures);

    // glm::vec3 positions[] = {
    //     glm::vec3(-2.0f, 0.0f, 0.0f),   glm::vec3(2.0f, 5.0f, -15.0f),
    //     glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    //     glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    //     glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    //     glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};
    TextureArray textures;
    textures.loadFromValue(glm::vec3(1.f), Texture::AMBIENT);
    textures.loadFromValue(glm::vec3(0.6f), Texture::DIFFUSE);
    textures.loadFromValue(glm::vec3(0.5f), Texture::SPECULAR);

    // for (int i = 0; i < 10; ++i) {
    //     glm::vec3 offset(0);
    //     offset[i % 3] = i % 5;
    //     addCube(positions[i] + offset, 1, 1, 1, textures);
    //     addSphere(positions[i], textures);
    // }

    addCube(glm::vec3(2, 3, 3), 1, 1, 1, textures);
    addSphere(glm::vec3(0, 6, 0), textures);

    // addmodel("resources/models/backpack/backpack.obj",
    //          glm::vec3(0.f, 6.f, 6.f));
}

Game::Game(const Settings& settings)
    : m_window(settings.width, settings.height, settings.title,
               settings.samples),
      m_frameBuffer(settings.width, settings.height, settings.samples),
      m_editorMode(settings.editor) {
    loadShaders();
    loadScene();
    uint32_t id = m_cameras.create<Camera>(
        0, 0, settings.width, settings.height, glm::vec3(-8.f, 9.f, 13.f));
    m_mainCamera = std::dynamic_pointer_cast<Camera>(m_cameras.get(id));
    m_mainCamera->setEulerAngle(
        glm::vec3(glm::radians(-15.f), glm::radians(-35.f), glm::radians(5.f)));

    m_app.systems.add<BoundingBoxSystem>();
    m_app.systems.add<TransformSystem>();

    m_window.setFramerateLimit(settings.frameRateLimit);
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
    auto entityIterEnd = m_app.entities.end();

    RenderStates states;
    std::vector<Ref<ShadowBuffer>> buffers;
    std::vector<const LightBase*> lightList;
    Light::Handle light;
    auto view = m_app.entities.getByComponents(light);
    auto end = view.end();
    // TODO: refactor this to deferred shading
    for (auto begin = view.begin(); begin != end; ++begin) {
        buffers.emplace_back(ShadowBuffer::create(1024, 1024));
        // draw depth map
        buffers.back()->beginScene(m_shaders.get("Shadow"), *light.get());
        for (auto cur = m_app.entities.begin(); cur != entityIterEnd; ++cur) {
            states.transform =
                m_app.entities.get(*cur)->component<Transform>()->getMatrix();
            m_app.entities.get(*cur)->draw(*buffers.back(), states);
        }
        buffers.back()->endScene();
        lightList.push_back(light.get());
    }

    // normal draw
    m_frameBuffer.beginScene();
    m_window.beginScene(m_shaders.get("Main"), m_mainCamera);
    m_window.setLighting(lightList, buffers);
    for (auto cur = m_app.entities.begin(); cur != entityIterEnd; ++cur) {
        states.transform =
            m_app.entities.get(*cur)->component<Transform>()->getMatrix();
        states.textures = m_app.entities.get(*cur)->getTextures();
        m_app.entities.get(*cur)->draw(m_window, states);
    }
    m_window.endScene();
    m_frameBuffer.endScene();

    if (m_editorMode) {
        Editor::instance().render();
    } else {
        m_frameBuffer.draw(m_shaders.get("FrameBuffer"));
    }

    m_window.display();
}

void Game::run(int minFps) {
    Clock clock;
    Time timeSinceLastUpdate = Time::Zero;
    Time timePerFrame = seconds(1.0 / minFps);
    Ref<Camera> camera = m_mainCamera;

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
