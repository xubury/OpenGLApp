#include <Component/Transform.hpp>

Transform::Transform() : m_transform(1.0) {}

void Transform::draw(RenderTarget &target) const {
    RenderStates states;
    states.transform = m_transform;
    m_axis.draw(target, states);
}

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

glm::vec3 Transform::getPosition() const {
    glm::vec3 pos(m_transform[2]);
    return pos;
}

void TransformSystem::update(EntityManager<DefaultEntity> &, const Time &) {}

void TransformSystem::draw(EntityManager<DefaultEntity> &manager,
                           RenderTarget &target) const {
    Transform::Handle transform;
    auto view = manager.getByComponents<Transform>(transform);
    auto end = view.end();
    for (auto cur = view.begin(); cur != end; ++cur) {
        transform->draw(target);
    }
}
