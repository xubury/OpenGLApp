#ifndef SANDBOX_LAYER_HPP
#define SANDBOX_LAYER_HPP

#include <string>

#include "Window/RenderWindow.hpp"
#include "Graphic/Shader.hpp"
#include "Graphic/Camera.hpp"
#include "ECS/ECS.hpp"
#include "Core/Time.hpp"
#include "Core/Layer.hpp"
#include "Graphic/ModelTextures.hpp"


class SandboxLayer : public te::Layer {
   public:
    SandboxLayer(int width, int height);

    void onUpdate(const te::Time &deltaTime) override;

    void onRender() override;

    void onAttach() override;

    void onEventPoll(const te::Event &event) override;

    void onEventProcess() override;

    void addSphere(const glm::vec3 &pos, float radius, const glm::vec3 &impulse,
                   const te::ModelTextures &textures);

    void addCube(const glm::vec3 &pos, float width, float height, float length,
                 const te::ModelTextures &textures, bool kinematic);

    void addModel(const std::string &path, const glm::vec3 &pos);

   private:
    void loadShaders();

    void loadScene();

   private:
    te::ShaderLibrary m_shaders;
    te::Ref<te::SceneManager<te::EntityBase>> m_scene;
    // TODO: make a follow camera
    te::Ref<te::Camera> m_camera;
};

#endif
