#include "Physics/PhysicsWorld.hpp"
#include "Physics/Collider.hpp"
#include "Physics/Rigidbody.hpp"

namespace te {

void PhysicsWorld::update(EntityManager<EntityBase> &manager,
                          const Time &deltaTime) {
    std::vector<ContactManifold> manifolds;
    CollisionObject::Handle collisonObj;
    auto view = manager.getByComponents<CollisionObject>(collisonObj);
    auto end = view.end();
    for (auto a = view.begin(); a != end; ++a) {
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

    for (const ContactManifold &manifold : manifolds) {
        Rigidbody *bodyA = dynamic_cast<Rigidbody *>(manifold.objA);
        Rigidbody *bodyB = dynamic_cast<Rigidbody *>(manifold.objB);

        const glm::vec3 &velA = bodyA ? bodyA->getVelocity() : glm::vec3(0);
        const glm::vec3 &velB = bodyB ? bodyB->getVelocity() : glm::vec3(0);
        const glm::vec3 &wA =
            bodyA ? bodyA->getAngularVelocity() : glm::vec3(0);
        const glm::vec3 &wB =
            bodyB ? bodyB->getAngularVelocity() : glm::vec3(0);

        for (uint8_t i = 0; i < manifold.pointCount; ++i) {
            float correction = 0.01f *
                               std::max(manifold.points[i].depth - 0.01, 0.0) /
                               deltaTime.count();
            const glm::vec3 rA =
                manifold.points[i].position - bodyA->getCenterOfMassInWorld();
            const glm::vec3 rB =
                manifold.points[i].position - bodyB->getCenterOfMassInWorld();
            float velRelative =
                glm::dot(velB + glm::cross(wB, rB) - velA - glm::cross(wA, rA),
                         manifold.normal);
            if (bodyB) {
                bodyB->addForce((correction - velRelative) * manifold.normal *
                                    bodyB->getMass() / deltaTime.count(),
                                manifold.points[i].position);
            }
        }
    }
    for (auto entity = view.begin(); entity != end; ++entity) {
        collisonObj->step(deltaTime);
    }
}

}  // namespace te
