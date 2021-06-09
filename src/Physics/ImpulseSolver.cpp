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

        const float massA = bodyA ? bodyA->getMass() : 1.0f;
        const float massB = bodyB ? bodyB->getMass() : 1.0f;

        const glm::vec3 &wA =
            bodyA ? bodyA->getAngularVelocity() : glm::vec3(0);
        const glm::vec3 &wB =
            bodyB ? bodyB->getAngularVelocity() : glm::vec3(0);

        for (uint8_t i = 0; i < manifold.pointCount; ++i) {
            const glm::vec3 rA = manifold.points[i].position -
                                 manifold.objA->owner()->getPosition() -
                                 bodyA->getCenterOfMass();
            const glm::vec3 rB = manifold.points[i].position -
                                 manifold.objB->owner()->getPosition() -
                                 bodyB->getCenterOfMass();
            glm::vec3 velRelative =
                velB + glm::cross(wB, rB) - velA - glm::cross(wA, rA);
            float speed = glm::dot(velRelative, manifold.normal);

            float e = (bodyA ? bodyA->getRestitution() : 1.0f) *
                      (bodyB ? bodyB->getRestitution() : 1.0f);
            float j = -(1.0f + e) * speed;
            // friction
            glm::vec3 tangent = velRelative - speed * manifold.normal;
            if (glm::length(tangent) > 0.0001f) {  // safe normalize
                tangent = glm::normalize(tangent);
            }

            float velFriction = glm::dot(velRelative, tangent);
            float staticFrictionA = bodyA ? bodyA->getStaticFriction() : 0.0f;
            float staticFrictionB = bodyB ? bodyB->getStaticFriction() : 0.0f;
            float dynamicFrictionA = bodyA ? bodyA->getDynamicFriction() : 0.0f;
            float dynamicFrictionB = bodyB ? bodyB->getDynamicFriction() : 0.0f;
            float mu = glm::length(glm::vec2(staticFrictionA, staticFrictionB));

            float f = -velFriction / (massA + massB);
            glm::vec3 friction;
            if (std::abs(f) < j * mu) {
                friction = f * tangent;
            } else {
                mu = glm::length(glm::vec2(dynamicFrictionA, dynamicFrictionB));
                friction = -j * tangent * mu;
            }
            if (bodyB) {
                bodyB->addForce((j * manifold.normal + friction) * massB /
                                    deltaTime.count(),
                                manifold.points[i].position);
            }
        }
    }
}

}  // namespace te
