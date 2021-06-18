#include "Core/Transform.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace te {

Transformable::Transformable() : m_transform(1.0) {}

void Transformable::transform(const glm::mat4 &transform) {
    m_transform = transform * m_transform;
}

void Transformable::translateLocal(const glm::vec3 &t) {
    m_transform = glm::translate(m_transform, t);
}

void Transformable::translateWorld(const glm::vec3 &t) {
    m_transform = glm::translate(glm::mat4(1.0), t) * m_transform;
}

void Transformable::rotateLocal(float radians, const glm::vec3 &axis) {
    m_transform = glm::rotate(m_transform, radians, axis);
}

void Transformable::rotateWorld(float radians, const glm::vec3 &axis) {
    m_transform = glm::rotate(glm::mat4(1.0), radians, axis) * m_transform;
}

glm::vec3 Transformable::getEulerAngle() const {
    glm::vec3 eulerAngle;
    if (m_transform[2][1] < 1.f) {
        if (m_transform[2][1] > -1.f) {
            eulerAngle.x = asin(-m_transform[2][1]);
            eulerAngle.y = atan2(m_transform[2][0], m_transform[2][2]);
            eulerAngle.z = atan2(m_transform[0][1], m_transform[1][1]);
        } else {
            eulerAngle.x = M_PI / 2;
            eulerAngle.y = -atan2(-m_transform[1][0], m_transform[0][0]);
            eulerAngle.z = 0;
        }
    } else {
        eulerAngle.x = -M_PI / 2;
        eulerAngle.y = atan2(-m_transform[1][0], m_transform[0][0]);
        eulerAngle.z = 0;
    }
    return eulerAngle;
}

void Transformable::setEulerAngle(glm::vec3 eulerAngle) {
    glm::vec4 &right = m_transform[0];
    glm::vec4 &up = m_transform[1];
    glm::vec4 &front = m_transform[2];
    right.x = cos(eulerAngle.y) * cos(eulerAngle.z) +
              sin(eulerAngle.x) * sin(eulerAngle.y) * sin(eulerAngle.z);
    right.y = cos(eulerAngle.x) * sin(eulerAngle.z);
    right.z = -cos(eulerAngle.z) * sin(eulerAngle.y) +
              cos(eulerAngle.y) * sin(eulerAngle.x) * sin(eulerAngle.z);

    front.x = cos(eulerAngle.x) * sin(eulerAngle.y);
    front.y = -sin(eulerAngle.x);
    front.z = cos(eulerAngle.x) * cos(eulerAngle.y);

    right = glm::normalize(right);
    front = glm::normalize(front);
    up = glm::vec4(glm::cross(glm::vec3(front), glm::vec3(right)), 0);
}

const glm::mat4 &Transformable::getTransform() const { return m_transform; }

void Transformable::setPosition(const glm::vec3 &position) {
    m_transform[3] = glm::vec4(position, 1.0);
}

glm::vec3 Transformable::getPosition() const { return m_transform[3]; }

glm::vec3 Transformable::getRight() const { return m_transform[0]; }

glm::vec3 Transformable::getUp() const { return m_transform[1]; }

glm::vec3 Transformable::getFront() const { return m_transform[2]; }

glm::vec3 Transformable::toLocalSpace(const glm::vec3 &world) const {
    glm::mat3 rotateInv = glm::transpose(m_transform);
    return rotateInv * (world - getPosition());
}

}  // namespace te
