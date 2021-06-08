#include "Physics/SphereCollider.hpp"

namespace te {

SphereCollider::SphereCollider(const glm::vec3& center, float radius)
    : m_center(center), m_radius(radius) {}

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

    float depth = dist - m_radius - sphere.m_radius;
    if (depth <= 0) {
        manifold.objA = owner()->component<CollisionObject>().get();
        manifold.objB = sphere.owner()->component<CollisionObject>().get();
        manifold.pointCount = 1;
        manifold.points[0].position = (aWorldPos + bWorldPos) / 2.f;
        manifold.points[0].depth = depth;
        manifold.normal = (bWorldPos - aWorldPos) / dist;
    }
    return manifold;
}

ContactManifold SphereCollider::testCollision(const HullCollider&) const {
    return ContactManifold();
}
}  // namespace te
