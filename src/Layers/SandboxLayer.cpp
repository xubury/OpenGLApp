#include "Layers/SandboxLayer.hpp"
#include "Apps/Application.hpp"
#include "Component/BoundingBox.hpp"
#include "Entity/Light.hpp"
#include "Entity/ModelEntity.hpp"
#include "Entity/Cube.hpp"
#include "Entity/Sphere.hpp"
#include "Physics/PhysicsWorld.hpp"
#include "Physics/Rigidbody.hpp"
#include "Physics/SphereCollider.hpp"
#include "Physics/HullCollider.hpp"
#include "Graphic/Renderer.hpp"

#include <iostream>

namespace te {

void SandboxLayer::addSphere(const glm::vec3& pos, float radius,
                             const glm::vec3& impulse,
                             const Ref<Material>& textures) {
    int id = m_scene->entities.create<Sphere>(radius, textures);
    EntityBase* sphere = m_scene->entities.get(id);
    sphere->add<Rigidbody>(10, true);
    sphere->add<SphereCollider>(glm::vec3(0), 1.0f);
    sphere->component<Rigidbody>()->addImpulse(impulse);
    sphere->setPosition(pos);
    sphere->setName("Sphere");
}

void SandboxLayer::addCube(const glm::vec3& pos, float width, float height,
                           float length, const Ref<Material>& textures,
                           bool kinematic) {
    int id = m_scene->entities.create<Cube>(width, height, length, textures);
    EntityBase* cube = m_scene->entities.get(id);
    cube->add<Rigidbody>(10, kinematic);
    cube->add<HullCollider>();
    MakeCubeCollider(*cube->component<HullCollider>().get(), width, height,
                     length);
    cube->setPosition(pos);
    cube->setName("Cube");
}

void SandboxLayer::addModel(const std::string& path, const glm::vec3& pos) {
    int id = m_scene->entities.create<ModelEntity>();
    EntityBase* model = m_scene->entities.get(id);
    dynamic_cast<ModelEntity*>(model)->loadFromFile(path);
    model->setPosition(pos);
    model->setName("model");
}

void SandboxLayer::loadShaders() {
    m_shaders.add("Main");
    m_shaders.get("Main")->loadFromFile("shader/vertex.glsl",
                                        "shader/fragment.glsl");
    m_shaders.get("Main")->bind();
    m_shaders.get("Main")->setVec3("uPointLight.position",
                                   glm::vec3(0.0f, 0.0f, 2.0f));
    m_shaders.get("Main")->setVec3("uPointLight.direction",
                                   glm::vec3(0.0f, 0.0f, -1.0f));
    m_shaders.get("Main")->setVec3("uPointLight.ambient", glm::vec3(0.5f));
    m_shaders.get("Main")->setVec3("uPointLight.diffuse", glm::vec3(0.5f));
    m_shaders.get("Main")->setVec3("uPointLight.specular", glm::vec3(1.0f));
    m_shaders.get("Main")->setFloat("uPointLight.constant", 1.0f);
    m_shaders.get("Main")->setFloat("uPointLight.linear", 0.09f);
    m_shaders.get("Main")->setFloat("uPointLight.quadratic", 0.032f);
    m_shaders.get("Main")->setFloat("uPointLight.cutOff",
                                    glm::cos(glm::radians(12.5f)));
    m_shaders.get("Main")->setFloat("uPointLight.outerCutOff",
                                    glm::cos(glm::radians(15.5f)));
}

void SandboxLayer::loadScene() {
    uint32_t lightSource = m_scene->entities.create<Light>();
    m_scene->entities.get(lightSource)->setName("Directional Light");
    auto light = dynamic_cast<Light*>(m_scene->entities.get(lightSource));
    light->add<ShadowMap>(10.0f);
    light->setPosition(glm::vec3(0, 8, 8));
    light->setEulerAngle(glm::vec3(glm::radians(45.f), glm::radians(180.f), 0));
    light->ambient = glm::vec3(0.5f);
    light->diffuse = glm::vec3(0.5f);
    light->specular = glm::vec3(0.5f);

    Ref<Material> textures = createRef<Material>();
    textures->loadFromValue(glm::vec3(1.f), Texture::AMBIENT);
    textures->loadFromValue(glm::vec3(0.6f), Texture::DIFFUSE);
    textures->loadFromValue(glm::vec3(0.5f), Texture::SPECULAR);

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
    Ref<Material> groundTextures = createRef<Material>();
    groundTextures->loadFromValue(glm::vec3(0.7f), Texture::AMBIENT);
    groundTextures->loadFromValue(glm::vec3(0.7f), Texture::DIFFUSE);
    groundTextures->loadFromValue(glm::vec3(0.f), Texture::SPECULAR);
    addCube(glm::vec3(0), 50, 1, 50, groundTextures, false);
}

SandboxLayer::SandboxLayer(int width, int height) : Layer("Sandbox") {
    m_camera = createRef<Camera>(0, 0, width, height);
    m_camera->setPosition(glm::vec3(-8.f, 15.f, 21.f));
    m_camera->setEulerAngle(glm::vec3(glm::radians(-25.f), glm::radians(-28.f),
                                      glm::radians(1.5f)));
    m_scene = createRef<SceneManager<EntityBase>>();
}

void SandboxLayer::onAttach() {
    loadShaders();
    loadScene();

    m_scene->systems.add<BoundingBoxSystem>();
    m_scene->systems.add<PhysicsWorld>();
    Application::instance().setMainCamera(m_camera);
    Application::instance().setActiveScene(m_scene);
}

void SandboxLayer::onUpdate(const Time& deltaTime) {
    m_scene->update(deltaTime);
}

void SandboxLayer::onRender() {
    Renderer::beginScene(Application::instance().getMainCamera(),
                         Application::instance().getFramebuffer());
    Renderer::clear();
    auto entityIterEnd = m_scene->entities.end();
    for (auto cur = m_scene->entities.begin(); cur != entityIterEnd; ++cur) {
        m_scene->entities.get(*cur)->draw(m_shaders.get("Main"));
    }
    Renderer::endScene();
}

void SandboxLayer::onEventPoll(const Event& event) {
    if (event.type == Event::KEY_PRESSED) {
        switch (event.key.code) {
            case Keyboard::T: {
                Ref<Material> textures = createRef<Material>();
                textures->loadFromValue(glm::vec3(1.f), Texture::AMBIENT);
                textures->loadFromValue(glm::vec3(0.6f), Texture::DIFFUSE);
                textures->loadFromValue(glm::vec3(0.5f), Texture::SPECULAR);
                addCube(glm::vec3(5, 5, 0), 1, 1, 1, textures, true);
            } break;
            default:
                break;
        }
    } else if (event.type == Event::RESIZED) {
        m_camera->setViewportSize(event.size.width, event.size.height);
    }
}

void SandboxLayer::onEventProcess() {}
}  // namespace te
