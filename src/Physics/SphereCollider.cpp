#include "Physics/SphereCollider.hpp"
#include "Physics/HullCollider.hpp"
#include "Physics/GJK.hpp"

namespace te {

SphereCollider::SphereCollider(const glm::vec3& center, float radius)
    : m_center(center), m_radius(radius) {}

glm::vec3 SphereCollider::findFurthestPoint(const glm::vec3& direction) const {
    return m_center + owner()->getPosition() +
           m_radius * glm::normalize(direction);
}

ContactManifold SphereCollider::testCollision(const Collider& collider) const {
    return collider.testCollision(*this);
}

ContactManifold SphereCollider::testCollision(
    const SphereCollider& sphere) const {
    ContactManifold manifold;
    glm::vec3 aWorldPos =
        owner()->component<Transform>()->getMatrix() * glm::vec4(m_center, 1.0);
    glm::vec3 bWorldPos = sphere.owner()->component<Transform>()->getMatrix() *
                          glm::vec4(sphere.m_center, 1.0);
    float dist = glm::length(aWorldPos - bWorldPos);

    float depth = m_radius + sphere.m_radius - dist;
    if (depth >= 0) {
        manifold.objA = owner()->component<CollisionObject>().get();
        manifold.objB = sphere.owner()->component<CollisionObject>().get();
        manifold.pointCount = 1;
        manifold.points[0].position = (aWorldPos + bWorldPos) / 2.f;
        manifold.points[0].depth = depth;
        manifold.normal = (bWorldPos - aWorldPos) / dist;
    }
    return manifold;
}

ContactManifold SphereCollider::testCollision(const HullCollider& hull) const {
    auto [collide, simlex] = gjk(*this, hull, 32);
    if (collide) {
        return epa(simlex, *this, hull, 32);
    }
    return {};
}

}  // namespace te
