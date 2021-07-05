#include "Physics/ImpulseSolver.hpp"
#include "Physics/Rigidbody.hpp"

namespace te {

void ImpulseSolver::solve(std::vector<ContactManifold> &manifolds,
                          const Time &deltaTime) {
    for (ContactManifold &manifold : manifolds) {
        Rigidbody *bodyA = dynamic_cast<Rigidbody *>(manifold.objA);
        Rigidbody *bodyB = dynamic_cast<Rigidbody *>(manifold.objB);

        const float invMassA = bodyA ? bodyA->getInvMass() : 1.0f;
        const float invMassB = bodyB ? bodyB->getInvMass() : 1.0f;

        glm::mat3 invInertiaA =
            bodyA ? bodyA->getGlobalInvInertia() : glm::mat3(1.0f);
        glm::mat3 invInertiaB =
            bodyB ? bodyB->getGlobalInvInertia() : glm::mat3(1.0f);

        const float e = (bodyA ? bodyA->getRestitution() : 1.0f) *
                        (bodyB ? bodyB->getRestitution() : 1.0f);
        const float friction = (bodyA ? bodyA->getDynamicFriction() : 0.f) *
                               (bodyB ? bodyB->getDynamicFriction() : 0.f);
        uint8_t iteration = 3;
        while (iteration-- > 0) {
            const glm::vec3 &vA = bodyA ? bodyA->getVelocity() : glm::vec3(0);
            const glm::vec3 &wA =
                bodyA ? bodyA->getAngularVelocity() : glm::vec3(0);
            const glm::vec3 &vB = bodyB ? bodyB->getVelocity() : glm::vec3(0);
            const glm::vec3 &wB =
                bodyB ? bodyB->getAngularVelocity() : glm::vec3(0);
            for (uint8_t i = 0; i < manifold.pointCount; ++i) {
                const glm::vec3 rA = manifold.points[i].positionA -
                                     manifold.objA->owner()->toWorldSpace(
                                         bodyA->getCenterOfMass());
                const glm::vec3 rB = manifold.points[i].positionB -
                                     manifold.objB->owner()->toWorldSpace(
                                         bodyB->getCenterOfMass());
                const glm::vec3 normal = manifold.points[i].normal;
                const glm::vec3 vR =
                    vB + glm::cross(wB, rB) - vA - glm::cross(wA, rA);

                // compute friction
                {
                    glm::vec3 t1, t2;
                    if (normal.x >= 0.57735f)
                        t1 = glm::vec3(normal.y, -normal.x, 0.0f);
                    else
                        t1 = glm::vec3(0.0f, normal.z, -normal.y);

                    t1 = glm::normalize(t1);
                    t2 = glm::cross(normal, t1);
                    const float t1Mass = glm::dot(
                        t1 * (invMassA + invMassB + invInertiaA + invInertiaB),
                        t1);
                    const float t2Mass = glm::dot(
                        t2 * (invMassA + invMassB + invInertiaA + invInertiaB),
                        t2);

                    const float vT1 = glm::dot(t1, vR);
                    const float vT2 = glm::dot(t2, vR);

                    float lambda1 = -vT1 / t1Mass;
                    float lambda2 = -vT2 / t2Mass;

                    const float maxFriction =
                        friction * manifold.points[i].normalImpusleSum;

                    const float lastImpulse1 =
                        manifold.points[i].tangentImpusleSum1;
                    const float lastImpulse2 =
                        manifold.points[i].tangentImpusleSum2;

                    manifold.points[i].tangentImpusleSum1 = std::clamp(
                        manifold.points[i].tangentImpusleSum1 + lambda1,
                        -maxFriction, maxFriction);
                    manifold.points[i].tangentImpusleSum2 = std::clamp(
                        manifold.points[i].tangentImpusleSum2 + lambda2,
                        -maxFriction, maxFriction);

                    lambda1 =
                        manifold.points[i].tangentImpusleSum1 - lastImpulse1;
                    lambda2 =
                        manifold.points[i].tangentImpusleSum2 - lastImpulse2;
                    const glm::vec3 impulse1 = t1 * lambda1;
                    const glm::vec3 impulse2 = t2 * lambda2;
                    if (bodyA && bodyA->isKinematic()) {
                        bodyA->addForce(-impulse1 / deltaTime.count(),
                                        manifold.points[i].positionA);
                        bodyA->addForce(-impulse2 / deltaTime.count(),
                                        manifold.points[i].positionA);
                    }
                    if (bodyB && bodyB->isKinematic()) {
                        bodyB->addForce(impulse1 / deltaTime.count(),
                                        manifold.points[i].positionB);
                        bodyB->addForce(impulse2 / deltaTime.count(),
                                        manifold.points[i].positionB);
                    }
                }
                // compute normal
                {
                    const float depth =
                        glm::dot(manifold.points[i].positionB -
                                     manifold.points[i].positionA,
                                 -manifold.points[i].normal);
                    const float vN = glm::dot(normal, vR);
                    const float dSlope = 0.005f;
                    const float rSlope = 0.5f;
                    float bias = -0.01f * std::max(depth - dSlope, 0.f) /
                                 deltaTime.count();
                    bias += e * std::min(vN + rSlope, 0.f);

                    const float normalMass =
                        glm::dot(normal * (invMassA + invMassB + invInertiaA +
                                           invInertiaB),
                                 normal);
                    float lambda = -(vN + bias) / normalMass;
                    float lastImpulse = manifold.points[i].normalImpusleSum;
                    manifold.points[i].normalImpusleSum = std::max(
                        0.f, manifold.points[i].normalImpusleSum + lambda);
                    lambda = manifold.points[i].normalImpusleSum - lastImpulse;
                    const glm::vec3 impulse = normal * lambda;
                    if (bodyA && bodyA->isKinematic()) {
                        bodyA->addForce(-impulse / deltaTime.count(),
                                        manifold.points[i].positionA);
                    }
                    if (bodyB && bodyB->isKinematic()) {
                        bodyB->addForce(impulse / deltaTime.count(),
                                        manifold.points[i].positionB);
                    }
                }
            }
        }
    }
}

}  // namespace te
