#include "Physics/PhysicsWorld.hpp"
#include "Physics/ImpulseSolver.hpp"
#include "Physics/PositionSolver.hpp"
#include "Physics/Collider.hpp"
#include "Physics/Rigidbody.hpp"
#include "Physics/Collision.hpp"

namespace te {

PhysicsWorld::PhysicsWorld() : m_gravity(0.f, -9.8f, 0.f) {
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
        return;
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
            if (body->isKinematic()) {
                body->addForce(m_gravity * body->getMass(),
                               body->getCenterOfMass());
            }
        }
        Transformable *transformA =
            dynamic_cast<Transformable *>(manager.get(a->id()));
        for (auto b = view.begin(); b != end; ++b) {
            if (a == b) continue;
            if (a->has<Collider>() && b->has<Collider>()) {
                Transformable *transformB =
                    dynamic_cast<Transformable *>(manager.get(b->id()));
                ContactManifold manifold = Collision::collide(
                    a->component<Collider>().get(), transformA,
                    b->component<Collider>().get(), transformB);
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
