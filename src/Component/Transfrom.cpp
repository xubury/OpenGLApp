#include <Component/Transform.hpp>
#include <glm/ext/matrix_transform.hpp>

Transform::Transform() : m_transform(1.0) {}

void Transform::transform(const glm::mat4 &transform) {
    m_transform = transform * m_transform;
}

void Transform::translate(const glm::vec3 &position) {
    m_transform = glm::translate(m_transform, position);
}

glm::vec3 Transform::getEulerAngle() const {
    glm::vec3 eulerAngle;
    if (m_transform[2][0] - 1 < std::numeric_limits<float>::epsilon()) {
        if (m_transform[2][0] + 1 > std::numeric_limits<float>::epsilon()) {
            eulerAngle.y = asin(m_transform[2][0]);
            eulerAngle.x = atan2(-m_transform[2][1], m_transform[2][2]);
            eulerAngle.z = atan2(-m_transform[1][0], m_transform[0][0]);
        } else {
            eulerAngle.y = -M_PI / 2;
            eulerAngle.x = -atan2(m_transform[0][1], m_transform[1][1]);
            eulerAngle.z = 0;
        }
    } else {
        eulerAngle.y = M_PI / 2;
        eulerAngle.x = atan2(m_transform[0][1], m_transform[1][1]);
        eulerAngle.z = 0;
    }
    eulerAngle.x = glm::degrees(eulerAngle.x);
    eulerAngle.y = glm::degrees(eulerAngle.y);
    eulerAngle.z = glm::degrees(eulerAngle.z);
    return eulerAngle;
}

void Transform::setEulerAngle(glm::vec3 eulerAngle) {
    eulerAngle.x = glm::radians(eulerAngle.x);
    eulerAngle.y = glm::radians(eulerAngle.y);
    eulerAngle.z = glm::radians(eulerAngle.z);

    glm::vec4 &right = m_transform[0];
    glm::vec4 &up = m_transform[1];
    glm::vec4 &front = m_transform[2];
    right.x = cos(eulerAngle.y) * cos((eulerAngle.z));
    right.y = cos(eulerAngle.z) * sin(eulerAngle.x) * sin(eulerAngle.y) +
              cos(eulerAngle.x) * sin(eulerAngle.z);
    right.z = -cos(eulerAngle.x) * cos(eulerAngle.z) * sin(eulerAngle.y) +
              sin(eulerAngle.x) * sin(eulerAngle.z);

    front.x = sin(eulerAngle.y);
    front.y = -cos(eulerAngle.y) * sin(eulerAngle.x);
    front.z = cos(eulerAngle.x) * cos(eulerAngle.y);

    right = glm::normalize(right);
    front = glm::normalize(front);
    up = glm::vec4(glm::cross(glm::vec3(front), glm::vec3(right)), 0);
}

glm::mat4 Transform::getTransform() const { return m_transform; }

void Transform::setPosition(const glm::vec3 &position) {
    m_transform[3] = glm::vec4(position, 1.0);
}

glm::vec3 Transform::getPosition() const {
    glm::vec3 pos(m_transform[3]);
    return pos;
}

void TransformComp::rotate(float angle, const glm::vec3 &axis) {
    m_transform = glm::rotate(m_transform, glm::radians(angle), axis);
}

Axis TransformSystem::s_axis;

TransformSystem::TransformSystem() { s_axis.create(); }

void TransformSystem::update(EntityManager<EntityBase> &, const Time &) {}

void TransformSystem::draw(EntityManager<EntityBase> &manager,
                           RenderTarget &target, RenderStates states) const {
    TransformComp::Handle transform;
    auto view = manager.getByComponents<TransformComp>(transform);
    auto end = view.end();
    for (auto cur = view.begin(); cur != end; ++cur) {
        states.transform = transform->getTransform();
        s_axis.draw(target, states);
    }
}
