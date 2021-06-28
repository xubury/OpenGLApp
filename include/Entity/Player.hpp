#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "ECS/ECS.hpp"
#include "Graphic/VertexArray.hpp"
#include "Graphic/Material.hpp"
#include "Window/ActionMap.hpp"
#include "Model/Model.hpp"
#include "Model/Animator.hpp"

namespace te {

class Player : public EntityBase {
   public:
    enum PlayerAction {
        MOVE_JUMP,
        MOVE_FORWARD,
        MOVE_BACKWARD,
        MOVE_LEFT,
        MOVE_RIGHT,
        MOUSE_MOVE
    };

   public:
    Player(EntityManager<EntityBase> *manager, uint32_t id);

    void update(const Time &deltaTime) override;

    void draw(const Shader &shader) const override;

    void move(PlayerAction movement);

   private:
    Ref<Model> m_model;
    ActionMap<int> m_inputs;
    Ref<Animator> m_animator;
    Ref<Animation> m_animation;

    Time m_timePerMove;
    Clock m_moveClock;

    glm::vec3 m_moveDir;

    bool m_isFirstMouse;
    glm::vec2 m_lastMousePos;
};

}  // namespace te

#endif /* PLAYER_HPP */
