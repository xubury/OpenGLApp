#include "Physics/PositionSolver.hpp"
#include "Physics/CollisionObject.hpp"
#include "Physics/Rigidbody.hpp"

namespace te {

void PositionSolver::solve(std::vector<ContactManifold> &manifolds,
                           const Time &) {
    for (const ContactManifold &manifold : manifolds) {
        Rigidbody *bodyA = dynamic_cast<Rigidbody *>(manifold.objA);
        Rigidbody *bodyB = dynamic_cast<Rigidbody *>(manifold.objB);

        const float invMassA = bodyA ? bodyA->getInvMass() : 1.0f;
        const float invMassB = bodyB ? bodyB->getInvMass() : 1.0f;

        for (uint32_t i = 0; i < manifold.pointCount; ++i) {
            const float percent = 0.8f;
            const float slop = 0.01f;
            float depth = glm::dot(
                manifold.points[i].positionB - manifold.points[i].positionA,
                -manifold.points[i].normal);
            glm::vec3 correction = manifold.points[i].normal * percent *
                                   std::max(depth - slop, 0.0f) /
                                   (invMassA + invMassB);
            if (bodyB && bodyB->isKinematic()) {
                bodyB->owner()->translateWorld(correction * invMassB);
            }
            if (bodyA && bodyA->isKinematic()) {
                bodyA->owner()->translateWorld(-correction * invMassA);
            }
        }
    }
}
}  // namespace te
