#include "Physics/PhysicsWorld.hpp"
#include "Physics/ImpulseSolver.hpp"
#include "Physics/PositionSolver.hpp"
#include "Physics/Collider.hpp"
#include "Physics/Rigidbody.hpp"

namespace te {

PhysicsWorld::PhysicsWorld() : m_gravity(0, -9.8, 0) {
    addSolver(createRef<ImpulseSolver>());
    addSolver(createRef<PositionSolver>());
}

void PhysicsWorld::addSolver(Ref<Solver> solver) {
    m_solvers.push_back(solver);
}

void PhysicsWorld::removeSolver(Ref<Solver> solver) {
    auto iter = std::find(m_solvers.begin(), m_solvers.end(), solver);
    if (iter == m_solvers.end()) {
        TE_CORE_WARN("Trying to remove a non-exixtent solver!");
    }
    m_solvers.erase(iter);
}

void PhysicsWorld::update(EntityManager<EntityBase> &manager,
                          const Time &deltaTime) {
    std::vector<ContactManifold> manifolds;
    CollisionObject::Handle collisonObj;
    auto view = manager.getByComponents<CollisionObject>(collisonObj);
    auto end = view.end();
    for (auto a = view.begin(); a != end; ++a) {
        if (collisonObj->isDynamic()) {
            Rigidbody *body = dynamic_cast<Rigidbody *>(collisonObj.get());
            body->addForce(m_gravity * body->getMass(),
                           body->getCenterOfMass());
        }
        for (auto b = view.begin(); b != end; ++b) {
            if (a == b) continue;
            if (a->has<Collider>() && b->has<Collider>()) {
                ContactManifold manifold =
                    a->component<Collider>()->testCollision(
                        *b->component<Collider>().get());
                if (manifold.hasCollision()) {
                    manifolds.emplace_back(manifold);
                }
            }
        }
    }

    solveManifolds(manifolds, deltaTime);

    for (auto entity = view.begin(); entity != end; ++entity) {
        collisonObj->step(deltaTime);
    }
}

void PhysicsWorld::solveManifolds(const std::vector<ContactManifold> &manifolds,
                                  const Time &deltaTime) {
    for (const auto &solver : m_solvers) {
        solver->solve(manifolds, deltaTime);
    }
}

}  // namespace te
