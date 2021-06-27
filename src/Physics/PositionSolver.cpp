#include "Physics/PositionSolver.hpp"
#include "Physics/CollisionObject.hpp"
#include "Physics/Rigidbody.hpp"

namespace te {

void PositionSolver::solve(const std::vector<ContactManifold> &manifolds,
                           const Time &) {
    for (const ContactManifold &manifold : manifolds) {
        Rigidbody *bodyA = dynamic_cast<Rigidbody *>(manifold.objA);
        Rigidbody *bodyB = dynamic_cast<Rigidbody *>(manifold.objB);

        const float massA = bodyA ? bodyA->getMass() : 1.0f;
        const float massB = bodyB ? bodyB->getMass() : 1.0f;

        for (uint32_t i = 0; i < manifold.pointCount; ++i) {
            const float percent = 0.8f;
            const float slop = 0.01f;
            glm::vec3 correction =
                manifold.normal * percent *
                std::max(manifold.points[i].depth - slop, 0.0f) /
                (massA + massB);
            if (bodyB) {
                if (bodyB->isKinematic()) {
                    bodyB->owner()->translateWorld(correction * massB / 2.f);
                }
            }
            if (bodyA) {
                if (bodyA->isKinematic()) {
                    bodyA->owner()->translateWorld(-correction * massA / 2.f);
                }
            }
        }
    }
}
}  // namespace te
