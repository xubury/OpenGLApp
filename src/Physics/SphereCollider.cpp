#include "Physics/SphereCollider.hpp"
#include "Physics/HullCollider.hpp"

namespace te {

SphereCollider::SphereCollider(const glm::vec3& center, float radius)
    : Collider(Type::SPHERE_COLLIDER), m_center(center), m_radius(radius) {}

glm::vec3 SphereCollider::findFurthestPoint(const glm::vec3& direction) const {
    return m_center + owner()->getPosition() +
           m_radius * glm::normalize(direction);
}

glm::vec3 SphereCollider::getCenter() const { return m_center; }

glm::vec3 SphereCollider::getCenterInWorld() const {
    return m_center + owner()->getPosition();
}

float SphereCollider::getRadius() const { return m_radius; }

}  // namespace te
