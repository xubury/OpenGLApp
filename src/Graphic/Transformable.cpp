#include <Graphic/Transformable.hpp>
#include <glm/ext/matrix_transform.hpp>

Transformable::Transformable() : m_transform(1.0f) {}

Transformable::~Transformable() {}

void Transformable::transform(const glm::mat4 &transform) {
    m_transform = transform * m_transform;
}

void Transformable::translate(const glm::vec3 &position) {
    m_transform = glm::translate(m_transform, position);
}

void Transformable::rotate(float angle, const glm::vec3 &axis) {
    m_transform = glm::rotate(m_transform, glm::radians(angle), axis);
}

glm::mat4 Transformable::getTransform() const { return m_transform; }

glm::vec3 Transformable::getPosition() const {
    glm::vec3 pos(m_transform[2]);
    return pos;
}
