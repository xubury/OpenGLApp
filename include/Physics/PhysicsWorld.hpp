#ifndef PHYSICS_WORLD_HPP
#define PHYSICS_WORLD_HPP

#include "ECS/ECS.hpp"
#include "Physics/CollisionObject.hpp"
#include "Physics/Solver.hpp"

namespace te {

class PhysicsWorld : public System<CollisionObject, EntityBase> {
   public:
    PhysicsWorld();

    void addSolver(Ref<Solver> solver);

    void removeSolver(Ref<Solver> solver);

    void update(EntityManager<EntityBase> &manager,
                const Time &deltaTime) override;

   private:
    void solveManifolds(const std::vector<ContactManifold> &manifolds,
                        const Time &deltaTime);
    std::vector<Ref<Solver>> m_solvers;
    glm::vec3 m_gravity;
};

}  // namespace te

#endif /* PHYSICS_WORLD_HPP */
