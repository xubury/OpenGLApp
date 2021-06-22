#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "ECS/ECS.hpp"
#include "Graphic/VertexArray.hpp"
#include "Graphic/Material.hpp"
#include "Window/ActionMap.hpp"
#include "Entity/PlayerCamera.hpp"
#include "Graphic/Model.hpp"

namespace te {

class Player : public EntityBase {
   public:
    enum Action {
        MOVE_JUMP,
        MOVE_FORWARD,
        MOVE_BACKWARD,
        MOVE_LEFT,
        MOVE_RIGHT
    };

   public:
    Player(EntityManager<EntityBase> *manager, uint32_t id);

    void draw(const Shader &shader) const override;

    void move(Action movement);

    void setPlayerCamera(PlayerCamera *camera) { m_camera = camera; }

    PlayerCamera *getPlayerCamera() { return m_camera; }

   private:
    Ref<Model> m_model;
    ActionMap<int> m_inputs;
    PlayerCamera *m_camera;
};

}  // namespace te

#endif /* PLAYER_HPP */
