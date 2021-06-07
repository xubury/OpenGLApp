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
                    TE_TRACE("Collide! id:{0} and id:{1}", a->id(), b->id());
                }
            }
        }
    }

    for (const ContactManifold &manifold : manifolds) {
        Rigidbody *bodyA = dynamic_cast<Rigidbody *>(manifold.objA.get());
        Rigidbody *bodyB = dynamic_cast<Rigidbody *>(manifold.objB.get());
        const float percent = 0.8f;
        const float slop = 0.01f;

        const float massA = bodyA ? bodyA->getMass() : 0.f;
        const float massB = bodyB ? bodyB->getMass() : 0.f;
        for (uint8_t i = 0; i < manifold.pointCount; ++i) {
            glm::vec3 correction =
                manifold.normal * percent *
                std::max(manifold.points[i].depth + slop, 0.0f) /
                (massA + massB);
            glm::vec3 deltaA = -massA * correction;
            glm::vec3 deltaB = massB * correction;
            TE_TRACE("massA:{0}, deltaA:{1} {2} {3}", massA, deltaA.x, deltaA.y,
                     deltaA.z);
            if (bodyA && bodyA->isKinematic()) {
                bodyA->addForce(deltaA, manifold.points[i].position);
            }
            if (bodyB && bodyB->isKinematic()) {
                bodyB->addForce(deltaB, manifold.points[i].position);
            }
        }
    }
    for (auto entity = view.begin(); entity != end; ++entity) {
        collisonObj->step(deltaTime);
    }
}

}  // namespace te
