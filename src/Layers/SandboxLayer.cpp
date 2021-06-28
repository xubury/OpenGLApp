#include "Layers/SandboxLayer.hpp"
#include "Apps/Application.hpp"
#include "Component/BoundingBox.hpp"
#include "Component/Light.hpp"
#include "Component/ShadowMap.hpp"
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
#include "Utils/File.hpp"

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
    const char* deferredVertex = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;
    
    out vec2 texCoord;
    
    void main() {
        texCoord = aTexCoord;
        gl_Position = vec4(aPos, 1.0);
    })";
    std::string deferredFragment;
    fileToString("shader/light.glsl", deferredFragment);
    m_shader = createScope<Shader>();
    m_shader->compile(deferredVertex, deferredFragment.c_str());

    float quadVertices[] = {
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f,  -1.0f, 0.0f, 1.0f, 0.0f,
    };
    Ref<VertexBuffer> buffer =
        createRef<VertexBuffer>(quadVertices, sizeof(quadVertices));
    buffer->setLayout({{ShaderDataType::Float3, "aPos"},
                       {ShaderDataType::Float2, "aTexCoord"}});
    m_quad = createScope<VertexArray>();
    m_quad->addVertexBuffer(buffer);
}

void SandboxLayer::loadScene() {
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    uint32_t lightSource = scene->entities.create<EntityBase>();
    auto light = scene->entities.get(lightSource);
    light->setName("Directional Light");
    light->add<ShadowMap>(20.f);
    light->setPosition(glm::vec3(0, 8, 8));
    light->setEulerAngle(glm::vec3(glm::radians(45.f), glm::radians(180.f), 0));

    light->add<DirectionalLight>();
    light->component<DirectionalLight>()->ambient = glm::vec3(0.5f);
    light->component<DirectionalLight>()->diffuse = glm::vec3(1.0f);
    light->component<DirectionalLight>()->specular = glm::vec3(1.0f);

    uint32_t pointLightId = scene->entities.create<EntityBase>();
    auto pointLight = scene->entities.get(pointLightId);
    pointLight->setName("Point Light");
    pointLight->setPosition(glm::vec3(0, 8, 8));
    pointLight->setEulerAngle(
        glm::vec3(glm::radians(90.f), glm::radians(180.f), 0));

    pointLight->add<PointLight>();
    pointLight->component<PointLight>()->ambient = glm::vec3(0.5f);
    pointLight->component<PointLight>()->diffuse = glm::vec3(1.0f);
    pointLight->component<PointLight>()->specular = glm::vec3(1.0f);

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
}

SandboxLayer::SandboxLayer(int width, int height)
    : Layer("Sandbox"), m_viewWidth(width), m_viewHeight(height) {}

void SandboxLayer::onAttach() {
    loadShaders();
    loadScene();
}

void SandboxLayer::onUpdate(const Time&) {}

void SandboxLayer::onRender() {
    Ref<SceneManager<EntityBase>> scene =
        Application::instance().getActiveScene();
    PointLight::Handle pointLight;
    auto view = scene->entities.getByComponents(pointLight);
    auto end = view.end();
    uint32_t index = 0;
    m_shader->bind();
    for (auto cur = view.begin(); cur != end; ++cur) {
        std::string prefix = "uPointLights[" + std::to_string(index) + "].";
        m_shader->setVec3(prefix + "position", cur->getPosition());
        m_shader->setVec3(prefix + "direction", cur->getFront());
        m_shader->setVec3(prefix + "ambient", pointLight->ambient);
        m_shader->setVec3(prefix + "diffuse", pointLight->diffuse);
        m_shader->setVec3(prefix + "specular", pointLight->specular);
        m_shader->setFloat(prefix + "constant", pointLight->constant);
        m_shader->setFloat(prefix + "linear", pointLight->linear);
        m_shader->setFloat(prefix + "quadratic", pointLight->quadratic);
        m_shader->setFloat(prefix + "cutOff", pointLight->cutOff);
        m_shader->setFloat(prefix + "outerCutOff", pointLight->outerCutOff);
        ++index;
    }
    m_shader->setInt("uLightCount", index);
    glDepthMask(GL_FALSE);
    Renderer::beginScene(*Application::instance().getMainCamera(),
                         Application::instance().getFramebuffer());
    Renderer::clear();
    Renderer::submit(*m_shader, *m_quad, GL_TRIANGLE_STRIP, false);
    Renderer::endScene();
    glDepthMask(GL_TRUE);
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
                textures->loadFromValue(0.5f * glm::vec3(r, g, b),
                                        Material::TEXTURE_AMBIENT);
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
                textures->loadFromValue(0.5f * glm::vec3(r, g, b),
                                        Material::TEXTURE_AMBIENT);
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
    }
}

void SandboxLayer::onEventProcess() {}

}  // namespace te
