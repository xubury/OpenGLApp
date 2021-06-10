#include "Game.hpp"
#include "Component/BoundingBox.hpp"
#include "Component/Transform.hpp"
#include "Component/Light.hpp"
#include "Entity/Camera.hpp"
#include "Entity/ModelEntity.hpp"
#include "Entity/Cube.hpp"
#include "Entity/Sphere.hpp"
#include "Editor/Editor.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Physics/Rigidbody.hpp"
#include "Physics/SphereCollider.hpp"
#include "Physics/HullCollider.hpp"

#include <iostream>

void Game::addSphere(const glm::vec3& pos, float radius,
                     const glm::vec3& impulse, const ModelTextures& textures) {
    int id = m_app.entities.create<Sphere>(radius, textures);
    EntityBase* sphere = m_app.entities.get(id);
    sphere->add<Rigidbody>(10, true);
    sphere->add<SphereCollider>(glm::vec3(0), 1.0f);
    sphere->component<Rigidbody>()->addImpulse(impulse);
    sphere->setPosition(pos);
    sphere->setName("Sphere");
}

void Game::addCube(const glm::vec3& pos, float width, float height,
                   float length, const ModelTextures& textures,
                   bool kinematic) {
    int id = m_app.entities.create<Cube>(width, height, length, textures);
    EntityBase* cube = m_app.entities.get(id);
    cube->add<Rigidbody>(10, kinematic);
    cube->add<HullCollider>();
    MakeCubeCollider(*cube->component<HullCollider>().get(), width, height,
                     length);
    cube->setPosition(pos);
    cube->setName("Cube");
}

void Game::addModel(const std::string& path, const glm::vec3& pos) {
    int id = m_app.entities.create<ModelEntity>();
    EntityBase* model = m_app.entities.get(id);
    dynamic_cast<ModelEntity*>(model)->load(path);
    model->setPosition(pos);
    model->setName("model");
}

void Game::loadShaders() {
    m_shaders.add("Main");
    m_shaders.get("Main")->loadFromFile("shader/vertex.glsl", "shader/fragment.glsl");
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

    ModelTextures textures;
    textures.loadFromValue(glm::vec3(1.f), Texture::AMBIENT);
    textures.loadFromValue(glm::vec3(0.6f), Texture::DIFFUSE);
    textures.loadFromValue(glm::vec3(0.5f), Texture::SPECULAR);

    glm::vec3 positions[] = {
        glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(2.0f, 11.0f, 0.f),
        glm::vec3(-4.0f, 8.0f, 0.0f), glm::vec3(2.0, 9.0f, -4.0f)};
    glm::vec3 impulse[] = {glm::vec3(0, 50, 0), glm::vec3(0, -50, 0),
                           glm::vec3(50, 0, 0), glm::vec3(0, 0, 50.f)};
    for (int i = 0; i < 4; ++i) {
        addSphere(positions[i], 1.0, impulse[i], textures);
    }

    addCube(glm::vec3(2, 3, 3), 1, 1, 1, textures, true);
    addCube(glm::vec3(2, 4, 0), 1, 1, 1, textures, true);
    // addModel("resources/models/backpack/backpack.obj",
    //          glm::vec3(0.f, 6.f, 6.f));

    // ground
    ModelTextures groundTextures;
    groundTextures.loadFromValue(glm::vec3(0.7f), Texture::AMBIENT);
    groundTextures.loadFromValue(glm::vec3(0.7f), Texture::DIFFUSE);
    groundTextures.loadFromValue(glm::vec3(0.f), Texture::SPECULAR);
    addCube(glm::vec3(0), 50, 1, 50, groundTextures, false);
}

Game::Game(const Settings& settings)
    : m_window(settings.width, settings.height, settings.title,
               settings.samples),
      m_screenLayer(settings.width, settings.height, settings.samples),
      m_editorMode(settings.editor) {
    loadShaders();
    loadScene();
    uint32_t id =
        m_app.entities.create<Camera>(0, 0, settings.width, settings.height);
    m_mainCamera = dynamic_cast<Camera*>(m_app.entities.get(id));
    m_mainCamera->setPosition(glm::vec3(-8.f, 15.f, 21.f));
    m_mainCamera->setEulerAngle(glm::vec3(
        glm::radians(-25.f), glm::radians(-28.f), glm::radians(1.5f)));

    m_app.systems.add<BoundingBoxSystem>();
    m_app.systems.add<TransformSystem>();
    m_app.systems.add<PhysicsWorld>();

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

    // std::vector<Ref<ShadowBuffer>> buffers;
    // std::vector<const LightBase*> lightList;
    // Light::Handle light;
    // auto view = m_app.entities.getByComponents(light);
    // auto end = view.end();
    // // TODO: refactor this to deferred shading
    // for (auto begin = view.begin(); begin != end; ++begin) {
    //     buffers.emplace_back(ShadowBuffer::create(1024, 1024));
    //     // draw depth map
    //     buffers.back()->beginScene(*light.get());
    //     for (auto cur = m_app.entities.begin(); cur != entityIterEnd; ++cur)
    //     {
    //         states.transform =
    //             m_app.entities.get(*cur)->component<Transform>()->getMatrix();
    //         m_app.entities.get(*cur)->draw(*buffers.back(), states);
    //     }
    //     buffers.back()->endScene();
    //     lightList.push_back(light.get());
    // }

    // normal draw

    m_screenLayer.begin();
    Renderer::clear();
    Renderer::beginScene(*m_mainCamera);
    m_shaders.get("Main")->bind();
    for (auto cur = m_app.entities.begin(); cur != entityIterEnd; ++cur) {
        const glm::mat4 model =
            m_app.entities.get(*cur)->component<Transform>()->getMatrix();
        m_app.entities.get(*cur)->draw(m_shaders.get("Main"), model);
    }
    Renderer::endScene();
    m_screenLayer.end();

    if (m_editorMode) {
        Editor::instance().render();
    }
    m_window.display();
}

void Game::run(int minFps) {
    Clock clock;
    Time timeSinceLastUpdate = Time::Zero;
    Time timePerFrame = seconds(1.0 / minFps);
    Camera* camera = m_mainCamera;

    if (m_editorMode) {
        Editor::instance().context.setScreenLayer(&m_screenLayer);
        Editor::instance().context.setWindow(&m_window);
        Editor::instance().context.setEntityManager(&m_app.entities);
        Editor::instance().context.setCamrea(camera);
    }
    while (!m_window.shouldClose()) {
        Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == Event::KEY_PRESSED) {
                switch (event.key.code) {
                    case Keyboard::T: {
                        ModelTextures textures;
                        textures.loadFromValue(glm::vec3(1.f),
                                               Texture::AMBIENT);
                        textures.loadFromValue(glm::vec3(0.6f),
                                               Texture::DIFFUSE);
                        textures.loadFromValue(glm::vec3(0.5f),
                                               Texture::SPECULAR);
                        addCube(glm::vec3(5, 5, 0), 1, 1, 1, textures, true);
                    } break;
                    case Keyboard::ESCAPE:
                        m_window.setShouldClose();
                        break;
                    default:
                        break;
                }
            } else if (event.type == Event::EventType::RESIZED) {
                // When minimized， the width and height will drop to zero
                if (event.size.width > 0 && event.size.height > 0) {
                    m_screenLayer.onResize(event.size.width, event.size.height);
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
