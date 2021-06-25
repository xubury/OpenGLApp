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
#include "Core/Math.hpp"
#include "Entity/Terrain.hpp"
#include "Entity/Player.hpp"
#include "Entity/PlayerCamera.hpp"

#include <iostream>

namespace te {

void SandboxLayer::addSphere(const glm::vec3& pos, float radius,
                             const glm::vec3& impulse,
                             const Ref<Material>& textures) {
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    int id = scene->entities.create<Sphere>(radius, textures);
    EntityBase* sphere = scene->entities.get(id);
    sphere->add<Rigidbody>(10, true);
    sphere->add<SphereCollider>(glm::vec3(0), 1.0f);
    sphere->component<Rigidbody>()->addImpulse(impulse);
    sphere->setPosition(pos);
    sphere->setName("Sphere");
}

void SandboxLayer::addCube(const glm::vec3& pos, float width, float height,
                           float length, const glm::vec3& impulse,
                           const Ref<Material>& textures, bool kinematic) {
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    int id = scene->entities.create<Cube>(width, height, length, textures);
    EntityBase* cube = scene->entities.get(id);
    cube->add<Rigidbody>(10, kinematic);
    cube->add<HullCollider>();
    MakeCubeCollider(*cube->component<HullCollider>().get(), width, height,
                     length);
    cube->component<Rigidbody>()->addImpulse(impulse);
    cube->setPosition(pos);
    cube->setName("Cube");
}

void SandboxLayer::addModel(const std::string& path, const glm::vec3& pos) {
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    int id = scene->entities.create<ModelEntity>();
    EntityBase* model = scene->entities.get(id);
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
                                   glm::vec3(0.f, 4.0f, 0.f));
    m_shaders.get("Main")->setVec3("uPointLight.direction",
                                   glm::vec3(0.0f, -1.0f, 0.0f));
    m_shaders.get("Main")->setVec3("uPointLight.ambient", glm::vec3(1.0f));
    m_shaders.get("Main")->setVec3("uPointLight.diffuse", glm::vec3(1.0f));
    m_shaders.get("Main")->setVec3("uPointLight.specular", glm::vec3(1.0f));
    m_shaders.get("Main")->setFloat("uPointLight.constant", 1.0f);
    m_shaders.get("Main")->setFloat("uPointLight.linear", 0.09f);
    m_shaders.get("Main")->setFloat("uPointLight.quadratic", 0.032f);
    m_shaders.get("Main")->setFloat("uPointLight.cutOff",
                                    glm::cos(glm::radians(12.5f)));
    m_shaders.get("Main")->setFloat("uPointLight.outerCutOff",
                                    glm::cos(glm::radians(67.5f)));
}

void SandboxLayer::loadScene() {
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    uint32_t lightSource = scene->entities.create<Light>();
    auto light = dynamic_cast<Light*>(scene->entities.get(lightSource));
    light->setName("Directional Light");
    light->add<ShadowMap>(20.f);
    light->setPosition(glm::vec3(0, 8, 8));
    light->setEulerAngle(glm::vec3(glm::radians(45.f), glm::radians(180.f), 0));
    light->ambient = glm::vec3(1.0f);
    light->diffuse = glm::vec3(1.0f);
    light->specular = glm::vec3(1.0f);

    // addModel("resources/models/backpack/backpack.obj",
    //          glm::vec3(0.f, 6.f, 6.f));

    // ground
    Ref<Material> groundTextures = createRef<Material>();
    groundTextures->loadFromValue(
        glm::vec3(0), Material::TEXTURE_AMBIENT,
        TextureParameter(GL_MIRRORED_REPEAT, GL_LINEAR));
    groundTextures->loadFromFile(
        "resources/terrain/sand_01_diff_4k.jpg", Material::TEXTURE_DIFFUSE,
        TextureParameter(GL_MIRRORED_REPEAT, GL_LINEAR));
    groundTextures->loadFromFile(
        "resources/terrain/sand_01_spec_4k.jpg", Material::TEXTURE_SPECULAR,
        TextureParameter(GL_MIRRORED_REPEAT, GL_LINEAR));

    scene->entities.create<Terrain>(10, 20, groundTextures);

    uint32_t playerId = scene->entities.create<Player>();
    scene->entities.get(playerId)->setPosition(glm::vec3(0.f, 5.f, 0.f));
    m_player = dynamic_cast<Player*>(scene->entities.get(playerId));

    uint32_t cameraId = scene->entities.create<PlayerCamera>(
        0, 0, m_viewWidth, m_viewHeight, m_player);
    m_camera = dynamic_cast<PlayerCamera*>(scene->entities.get(cameraId));
    m_player->setPlayerCamera(m_camera);
    Application::instance().setSceneCamera(m_camera);
}

SandboxLayer::SandboxLayer(int width, int height)
    : Layer("Sandbox"), m_viewWidth(width), m_viewHeight(height) {}

void SandboxLayer::onAttach() {
    loadShaders();
    loadScene();
}

void SandboxLayer::onUpdate(const Time& deltaTime) {
    m_camera->update(deltaTime);
    m_player->update(deltaTime);
}

void SandboxLayer::onRender() {
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    Renderer::beginScene(*Application::instance().getMainCamera(),
                         Application::instance().getFramebuffer());
    Renderer::clear();
    auto entityIterEnd = scene->entities.end();
    for (auto cur = scene->entities.begin(); cur != entityIterEnd; ++cur) {
        scene->entities.get(*cur)->draw(*m_shaders.get("Main"));
    }
    Renderer::endScene();
}

void SandboxLayer::onEventPoll(const Event& event) {
    if (event.type == Event::KEY_PRESSED) {
        switch (event.key.code) {
            case Keyboard::T: {
                RandomGenerator random;

                Ref<Material> textures = createRef<Material>();
                float r = random.rnd(0.5, 1.0);
                float g = random.rnd(0.5, 1.0);
                float b = random.rnd(0.5, 1.0);
                textures->loadFromValue(glm::vec3(r, g, b),
                                        Material::TEXTURE_DIFFUSE);
                textures->loadFromValue(glm::vec3(0.5f),
                                        Material::TEXTURE_SPECULAR);
                glm::vec3 impulse(random.rnd(-25, 25), random.rnd(-25, 25),
                                  random.rnd(-25, 25));
                addSphere(glm::vec3(5, 15, 0), 1, impulse, textures);
            } break;
            case Keyboard::R: {
                RandomGenerator random;

                Ref<Material> textures = createRef<Material>();
                float r = random.rnd(0.5, 1.0);
                float g = random.rnd(0.5, 1.0);
                float b = random.rnd(0.5, 1.0);
                textures->loadFromValue(glm::vec3(r, g, b),
                                        Material::TEXTURE_DIFFUSE);
                textures->loadFromValue(glm::vec3(0.5f),
                                        Material::TEXTURE_SPECULAR);
                glm::vec3 impulse(random.rnd(-25, 25), random.rnd(-25, 25),
                                  random.rnd(-25, 25));
                addCube(glm::vec3(5, 15, 0), 1, 1, 1, impulse, textures, true);
            } break;
            default:
                break;
        }
    } else if (event.type == Event::RESIZED) {
        m_camera->setViewportSize(event.size.width, event.size.height);
    }
    m_camera->processEvent(event);
}

void SandboxLayer::onEventProcess() { m_camera->processEvents(); }

}  // namespace te
