#ifndef PHYSICS_WORLD_HPP
#define PHYSICS_WORLD_HPP

#include "ECS/ECS.hpp"
#include "Physics/CollisionObject.hpp"

namespace te {

class PhysicsWorld : public System<CollisionObject, EntityBase> {
   public:
    PhysicsWorld() = default;
    void update(EntityManager<EntityBase> &manager,
                const Time &deltaTime) override;
};

}  // namespace te

#endif /* PHYSICS_WORLD_HPP */
