#include <Component/Transform.hpp>

Transform::Transform() : m_transform(1.0) {}

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

void Transform::setPosition(const glm::vec3 &position) {
    m_transform[3] = glm::vec4(position, 1.0);
}

glm::vec3 Transform::getPosition() const {
    glm::vec3 pos(m_transform[3]);
    return pos;
}

Axis TransformSystem::s_axis;

TransformSystem::TransformSystem() { s_axis.create(); }

void TransformSystem::update(EntityManager<EntityBase> &, const Time &) {}

void TransformSystem::draw(EntityManager<EntityBase> &manager,
                           RenderTarget &target, RenderStates states) const {
    Transform::Handle transform;
    auto view = manager.getByComponents<Transform>(transform);
    auto end = view.end();
    for (auto cur = view.begin(); cur != end; ++cur) {
        states.transform = transform->getTransform();
        s_axis.draw(target, states);
    }
}
