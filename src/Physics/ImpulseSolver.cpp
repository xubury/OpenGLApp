#include "Physics/ImpulseSolver.hpp"
#include "Physics/Rigidbody.hpp"

namespace te {

void ImpulseSolver::solve(const std::vector<ContactManifold> &manifolds,
                          const Time &deltaTime) {
    for (const ContactManifold &manifold : manifolds) {
        Rigidbody *bodyA = dynamic_cast<Rigidbody *>(manifold.objA);
        Rigidbody *bodyB = dynamic_cast<Rigidbody *>(manifold.objB);

        const glm::vec3 &vA = bodyA ? bodyA->getVelocity() : glm::vec3(0);
        const glm::vec3 &vB = bodyB ? bodyB->getVelocity() : glm::vec3(0);
        const float invMassA = bodyA ? bodyA->getInvMass() : 1.0f;
        const float invMassB = bodyB ? bodyB->getInvMass() : 1.0f;
        const glm::vec3 &wA =
            bodyA ? bodyA->getAngularVelocity() : glm::vec3(0);
        const glm::vec3 &wB =
            bodyB ? bodyB->getAngularVelocity() : glm::vec3(0);

        glm::mat3 mInvA(invMassA);
        glm::mat3 iInvA =
            bodyA ? bodyA->getGlobalInvInertia() : glm::mat3(1.0f);
        glm::mat3 mInvB(invMassB);
        glm::mat3 iInvB =
            bodyB ? bodyB->getGlobalInvInertia() : glm::mat3(1.0f);

        for (uint8_t i = 0; i < manifold.pointCount; ++i) {
            const glm::vec3 rA = manifold.objA->owner()->toWorldVector(
                manifold.points[i].positionA - bodyA->getCenterOfMass());
            const glm::vec3 rB = manifold.objB->owner()->toWorldVector(
                manifold.points[i].positionB - bodyB->getCenterOfMass());
            glm::vec3 jVA = -manifold.points[i].normal;
            glm::vec3 jWA = glm::cross(-rA, manifold.points[i].normal);
            glm::vec3 jVB = manifold.points[i].normal;
            glm::vec3 jWB = glm::cross(rB, manifold.points[i].normal);
            const glm::vec3 b(0);

            const glm::vec3 jv_b =
                -(jVA * vA + jWA * wA + jVB * vB + jWB * wB + b);
            float jmj = glm::dot(jVA * mInvA, jVA) +
                        glm::dot(jWA * iInvA, jWA) +
                        glm::dot(jVB * mInvB, jVB) + glm::dot(jWB * iInvB, jWB);
            glm::vec3 lambda = jv_b / jmj;
            if (bodyB && bodyB->isKinematic()) {
                bodyB->addImpulse(jVB * lambda);
                bodyB->addAngularImpulse(jWB * lambda);
            }
            if (bodyA && bodyA->isKinematic()) {
                bodyA->addImpulse(jVA * lambda);
                bodyA->addAngularImpulse(jWA * lambda);
            }
        }
    }
}

}  // namespace te
