#include "Physics/ImpulseSolver.hpp"
#include "Physics/Rigidbody.hpp"

namespace te {
void ImpulseSolver::solve(const std::vector<ContactManifold> &manifolds,
                          const Time &deltaTime) {
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
            float correction =
                0.01f * manifold.points[i].depth / deltaTime.count();
            const glm::vec3 rA =
                manifold.points[i].position - bodyA->getCenterOfMassInWorld();
            const glm::vec3 rB =
                manifold.points[i].position - bodyB->getCenterOfMassInWorld();
            float velRelative =
                glm::dot(velB + glm::cross(wB, rB) - velA - glm::cross(wA, rA),
                         manifold.normal);

            float e = (bodyA ? bodyA->getRestitution() : 1.0f) *
                      (bodyB ? bodyB->getRestitution() : 1.0f);
            glm::vec3 deltaVel =
                (correction - (1.0f + e) * velRelative) * manifold.normal;
            if (bodyB) {
                bodyB->addForce(deltaVel * bodyB->getMass() / deltaTime.count(),
                                manifold.points[i].position);
            }
        }
    }
}
}  // namespace te
