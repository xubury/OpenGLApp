#ifndef SANDBOX_LAYER_HPP
#define SANDBOX_LAYER_HPP

#include <string>

#include "Window/RenderWindow.hpp"
#include "Graphic/Shader.hpp"
#include "ECS/ECS.hpp"
#include "Core/Time.hpp"
#include "Core/Layer.hpp"
#include "Graphic/Material.hpp"
#include "Entity/PlayerCamera.hpp"
#include "Graphic/VertexArray.hpp"

namespace te {

class SandboxLayer : public Layer {
   public:
    SandboxLayer(int width, int height);

    void onUpdate(const Time &deltaTime) override;

    void onRender() override;

    void onAttach() override;

    void onEventPoll(const Event &event) override;

    void onEventProcess() override;

    void addSphere(const glm::vec3 &pos, float radius, const glm::vec3 &impulse,
                   const Ref<Material> &textures);

    void addCube(const glm::vec3 &pos, float width, float height, float length,
                 const glm::vec3 &impulse, const Ref<Material> &textures,
                 bool kinematic);

    void addModel(const std::string &path, const glm::vec3 &pos);

   private:
    void loadShaders();

    void loadScene();

   private:
    uint32_t m_viewWidth;
    uint32_t m_viewHeight;
    Scope<Shader> m_shader;
    Scope<VertexArray> m_quad;
    PlayerCamera *m_camera;
    Player *m_player;
};

}  // namespace te

#endif
