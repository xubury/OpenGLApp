#include "Physics/ImpulseSolver.hpp"
#include "Physics/Rigidbody.hpp"

namespace te {

void ImpulseSolver::initConstraint(std::vector<ContactManifold> &manifolds) {
    for (ContactManifold &manifold : manifolds) {
        for (uint8_t i = 0; i < manifold.pointCount; ++i) {
            if (manifold.points[i].normal.x >= 0.57735f)
                manifold.points[i].tangent1 =
                    glm::vec3(manifold.points[i].normal.y,
                              -manifold.points[i].normal.x, 0.0f);
            else
                manifold.points[i].tangent1 =
                    glm::vec3(0.0f, manifold.points[i].normal.z,
                              -manifold.points[i].normal.y);

            manifold.points[i].tangent1 =
                glm::normalize(manifold.points[i].tangent1);
            manifold.points[i].tangent2 = glm::cross(
                manifold.points[i].normal, manifold.points[i].tangent1);
            manifold.points[i].depth = glm::dot(
                manifold.points[i].positionB - manifold.points[i].positionA,
                -manifold.points[i].normal);
        }
    }
}

void ImpulseSolver::solveConstraint(ContactManifold &manifold,
                                    const Time &deltaTime) {
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
    const glm::vec3 &vA = bodyA ? bodyA->getVelocity() : glm::vec3(0);
    const glm::vec3 &wA = bodyA ? bodyA->getAngularVelocity() : glm::vec3(0);
    const glm::vec3 &vB = bodyB ? bodyB->getVelocity() : glm::vec3(0);
    const glm::vec3 &wB = bodyB ? bodyB->getAngularVelocity() : glm::vec3(0);
    for (uint8_t i = 0; i < manifold.pointCount; ++i) {
        glm::vec3 impulse(0.f);
        const glm::vec3 rA =
            manifold.points[i].positionA -
            manifold.objA->owner()->toWorldSpace(bodyA->getCenterOfMass());
        const glm::vec3 rB =
            manifold.points[i].positionB -
            manifold.objB->owner()->toWorldSpace(bodyB->getCenterOfMass());
        const glm::vec3 &normal = manifold.points[i].normal;
        const glm::vec3 vR = vB + glm::cross(wB, rB) - vA - glm::cross(wA, rA);
        // compute normal
        {
            const float vN = glm::dot(normal, vR);
            const float dSlope = 0.005f;
            const float rSlope = 0.5f;
            float bias = -1.0f *
                         std::max(manifold.points[i].depth - dSlope, 0.f) /
                         deltaTime.count();
            bias += e * std::min(vN + rSlope, 0.f);

            const float normalMass = glm::dot(
                normal * (invMassA + invMassB + invInertiaA + invInertiaB),
                normal);
            float lambda = -(vN + bias) / normalMass;
            float lastImpulse = manifold.points[i].normalImpusleSum;
            manifold.points[i].normalImpusleSum =
                std::max(0.f, manifold.points[i].normalImpusleSum + lambda);
            lambda = manifold.points[i].normalImpusleSum - lastImpulse;
            impulse += normal * lambda;
        }
        // compute friction
        {
            const glm::vec3 &t1 = manifold.points[i].tangent1;
            const glm::vec3 &t2 = manifold.points[i].tangent2;
            const float t1Mass = glm::dot(
                t1 * (invMassA + invMassB + invInertiaA + invInertiaB), t1);
            const float t2Mass = glm::dot(
                t2 * (invMassA + invMassB + invInertiaA + invInertiaB), t2);

            const float vT1 = glm::dot(t1, vR);
            const float vT2 = glm::dot(t2, vR);

            float lambda1 = -vT1 / t1Mass;
            float lambda2 = -vT2 / t2Mass;

            const float maxFriction =
                friction * manifold.points[i].normalImpusleSum;

            const float lastImpulse1 = manifold.points[i].tangentImpusleSum1;
            const float lastImpulse2 = manifold.points[i].tangentImpusleSum2;

            manifold.points[i].tangentImpusleSum1 =
                std::clamp(manifold.points[i].tangentImpusleSum1 + lambda1,
                           -maxFriction, maxFriction);
            manifold.points[i].tangentImpusleSum2 =
                std::clamp(manifold.points[i].tangentImpusleSum2 + lambda2,
                           -maxFriction, maxFriction);

            lambda1 = manifold.points[i].tangentImpusleSum1 - lastImpulse1;
            lambda2 = manifold.points[i].tangentImpusleSum2 - lastImpulse2;
            impulse += t1 * lambda1 + t2 * lambda2;
        }
        if (bodyA && bodyA->isKinematic()) {
            bodyA->addImpulse(-impulse);
            bodyA->addAngularImpulse(-glm::cross(rA, impulse));
        }
        if (bodyB && bodyB->isKinematic()) {
            bodyB->addImpulse(impulse);
            bodyB->addAngularImpulse(glm::cross(rB, impulse));
        }
    }
}

void ImpulseSolver::solve(std::vector<ContactManifold> &manifolds,
                          const Time &deltaTime) {
    initConstraint(manifolds);
    for (ContactManifold &manifold : manifolds) {
        solveConstraint(manifold, deltaTime);
    }
    m_cachedManifolds = manifolds;
}

}  // namespace te
