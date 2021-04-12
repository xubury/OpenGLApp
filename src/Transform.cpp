#include "Transform.hpp"
#include <glm/ext/matrix_transform.hpp>

Transform::Transform() : m_transform(1.0f) {}

void Transform::transform(const glm::mat4 &transform) {
    m_transform = transform * m_transform;
}

void Transform::translate(const glm::vec3 &position) {
    m_transform = glm::translate(m_transform, position);
}

void Transform::rotate(float angle, const glm::vec3 &axis) {
    m_transform = glm::rotate(m_transform, glm::radians(angle), axis);
}

glm::mat4 Transform::getTransform() const { return m_transform; }
