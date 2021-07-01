#include "Physics/SphereCollider.hpp"
#include "Physics/HullCollider.hpp"

namespace te {

SphereCollider::SphereCollider(const glm::vec3 &center, float radius)
    : Collider(Type::SPHERE_COLLIDER), m_center(center), m_radius(radius) {}

glm::vec3 SphereCollider::findFurthestPoint(
    const glm::vec3 &direction, const Transformable &transform) const {
    return m_center + transform.getPosition() +
           m_radius * transform.getScale() * glm::normalize(direction);
}

glm::vec3 SphereCollider::getCenter() const { return m_center; }

float SphereCollider::getRadius() const { return m_radius; }

}  // namespace te
