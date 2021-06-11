#include "Physics/PositionSolver.hpp"
#include "Physics/CollisionObject.hpp"
#include "Physics/Rigidbody.hpp"

namespace te {

void PositionSolver::solve(const std::vector<ContactManifold> &manifolds,
                           const Time &) {
    for (const ContactManifold &manifold : manifolds) {
        Rigidbody *bodyA = dynamic_cast<Rigidbody *>(manifold.objA);
        Rigidbody *bodyB = dynamic_cast<Rigidbody *>(manifold.objB);

        float massA = bodyA ? bodyA->getMass() : 0.0f;
        float massB = bodyB ? bodyB->getMass() : 0.0f;

        const float percent = 0.8f;
        const float slop = 0.01f;

        glm::vec3 correction = manifold.normal * percent *
                               std::max(manifold.points[0].depth - slop, 0.0f) /
                               (massA + massB);

        if (bodyB) {
            if (bodyB->isKinematic()) {
                bodyB->owner()->translateWorld(correction * massB);
            }
        }
        if (bodyA) {
            if (bodyA->isKinematic()) {
                bodyA->owner()->translateWorld(-correction * massA);
            }
        }
    }
}
}  // namespace te
