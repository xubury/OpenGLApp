#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "ECS/ECS.hpp"
#include "Graphic/VertexArray.hpp"
#include "Graphic/Material.hpp"
#include "Window/ActionMap.hpp"

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

   private:
    Ref<VertexArray> m_vertexArray;
    Ref<Material> m_material;
    ActionMap<int> m_inputs;
};

}  // namespace te

#endif /* PLAYER_HPP */
