#ifndef SANDBOX_LAYER_HPP
#define SANDBOX_LAYER_HPP

#include <string>

#include "Window/RenderWindow.hpp"
#include "Graphic/Shader.hpp"
#include "ECS/ECS.hpp"
#include "Core/Time.hpp"
#include "Core/Layer.hpp"
#include "ECS/SceneManager.hpp"
#include "Graphic/ModelTextures.hpp"

using namespace te;

class SandboxLayer : public Layer {
   public:
    SandboxLayer();

    void onUpdate(const Time &deltaTime) override;

    void onRender() override;

    void onAttach() override;

    void onEventPoll(const Event &event) override;

    void onEventProcess() override;

    void addSphere(const glm::vec3 &pos, float radius, const glm::vec3 &impulse,
                   const ModelTextures &textures);

    void addCube(const glm::vec3 &pos, float width, float height, float length,
                 const ModelTextures &textures, bool kinematic);

    void addModel(const std::string &path, const glm::vec3 &pos);

    EntityManager<EntityBase> *getEntityManager() { return &m_scene.entities; }

   private:
    void loadShaders();

    void loadScene();

   private:
    ShaderLibrary m_shaders;
    SceneManager<EntityBase> m_scene;
};

#endif
