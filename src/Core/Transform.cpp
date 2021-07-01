#include "Core/Transform.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>

namespace te {

Transformable::Transformable()
    : m_position(0.f), m_rotation(1.f, 0.f, 0.f, 0.f), m_scale(1.0f) {}

void Transformable::setTransform(const glm::mat4 &transform) {
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(transform, m_scale, m_rotation, m_position, skew,
                   perspective);
}

void Transformable::setRotation(const glm::quat &rotation) {
    m_rotation = rotation;
}

glm::quat Transformable::getRotation() const { return m_rotation; }

void Transformable::translateLocal(const glm::vec3 &t) {
    m_position += m_rotation * t;
}

void Transformable::translateWorld(const glm::vec3 &t) { m_position += t; }

void Transformable::rotateLocal(float radians, const glm::vec3 &axis) {
    m_rotation = m_rotation * glm::angleAxis(radians, axis);
}

void Transformable::rotateWorld(float radians, const glm::vec3 &axis) {
    m_rotation = glm::angleAxis(radians, axis) * m_rotation;
}

void Transformable::rotateLocal(const glm::quat &rotation) {
    m_rotation = m_rotation * rotation;
}

void Transformable::rotateWorld(const glm::quat &rotation) {
    m_rotation = rotation * m_rotation;
}

glm::vec3 Transformable::getEulerAngle() const {
    return glm::eulerAngles(m_rotation);
}

void Transformable::setEulerAngle(glm::vec3 eulerAngle) {
    m_rotation = glm::quat(eulerAngle);
}

glm::mat4 Transformable::getTransform() const {
    float angle = glm::angle(m_rotation);
    glm::vec3 axis = glm::axis(m_rotation);

    glm::mat4 mat(1);

    mat = glm::translate(mat, m_position);
    mat = glm::rotate(mat, angle, axis);
    mat = glm::scale(mat, m_scale);

    return mat;
}

void Transformable::setPosition(const glm::vec3 &position) {
    m_position = position;
}

glm::vec3 Transformable::getPosition() const { return m_position; }

glm::vec3 Transformable::getLeft() const {
    return m_rotation * glm::vec3(1.f, 0.f, 0.f);
}

glm::vec3 Transformable::getUp() const {
    return m_rotation * glm::vec3(0.f, 1.f, 0.f);
}

glm::vec3 Transformable::getFront() const {
    return m_rotation * glm::vec3(0.f, 0.f, 1.f);
}

glm::vec3 Transformable::getScale() const { return m_scale; }

glm::vec3 Transformable::toLocalSpace(const glm::vec3 &world) const {
    glm::mat3 rotation = glm::toMat3(m_rotation);
    return glm::transpose(rotation) * (world - getPosition());
}

glm::vec3 Transformable::toWorldSpace(const glm::vec3 &local) const {
    return m_rotation * local + m_position;
}

glm::vec3 Transformable::toLocalVector(const glm::vec3 &worldVec) const {
    glm::mat3 rotation = glm::toMat3(m_rotation);
    return glm::transpose(rotation) * worldVec;
}

glm::vec3 Transformable::toWorldVector(const glm::vec3 &localVec) const {
    return m_rotation * localVec;
}

}  // namespace te
