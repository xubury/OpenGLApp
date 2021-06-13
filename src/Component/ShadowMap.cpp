#include "Component/ShadowMap.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace te {

ShadowMap::ShadowMap(float dist) : m_shadowDist(dist) {}

void ShadowMap::computeCameraBound(const Ref<Camera> &camera) {
    glm::mat3 transform =
        glm::transpose(owner()->getTransform()) * camera->getTransform();
    glm::vec3 camPos =
        camera->getPosition() - owner()->getFront() * m_shadowDist;

    const float nearZ = camera->getNearZ();
    const float farZ = camera->getFarZ();
    const float fov = camera->getFOV();
    const float aspect = camera->getAspect();
    const float nearHeight = nearZ * std::tan(glm::radians(fov));
    const float nearWidth = nearHeight * aspect;
    const float farHeight = farZ * std::tan(glm::radians(fov));
    const float farWidth = farHeight * aspect;
    std::vector<glm::vec3> vertices(8);
    vertices[0] =
        transform * (glm::vec3(-nearWidth, -nearHeight, -nearZ) + camPos);
    vertices[1] =
        transform * (glm::vec3(-nearWidth, nearHeight, -nearZ) + camPos);
    vertices[2] =
        transform * (glm::vec3(nearWidth, -nearHeight, -nearZ) + camPos);
    vertices[3] =
        transform * (glm::vec3(nearWidth, nearHeight, -nearZ) + camPos);

    vertices[4] =
        transform * (glm::vec3(-farWidth, -farHeight, -farZ) + camPos);
    vertices[5] = transform * (glm::vec3(-farWidth, farHeight, -farZ) + camPos);
    vertices[6] = transform * (glm::vec3(farWidth, -farHeight, -farZ) + camPos);
    vertices[7] = transform * (-glm::vec3(farWidth, farHeight, -farZ) + camPos);
    glm::vec3 min(std::numeric_limits<float>::max());
    glm::vec3 max(std::numeric_limits<float>::min());
    for (int i = 0; i < 8; ++i) {
        min.x = std::min(min.x, vertices[i].x);
        min.y = std::min(min.y, vertices[i].y);
        min.z = std::min(min.z, vertices[i].z);
        max.x = std::max(max.x, vertices[i].x);
        max.y = std::max(max.y, vertices[i].y);
        max.z = std::max(max.z, vertices[i].z);
    }
    setFrustum(max - min);
}

void ShadowMap::setFrustum(const glm::vec3 &size) {
    const glm::vec3 &up = owner()->getUp();
    const glm::vec3 &front = owner()->getFront();
    const glm::vec3 &pos = owner()->getPosition();
    glm::mat4 lightView = glm::lookAt(pos, pos + front, up);
    glm::mat4 lightProjection =
        glm::ortho(-size.x / 2.f, size.x / 2.f, -size.y / 2.f, size.y / 2.f,
                   -size.z / 2.f, size.z / 2.f);
    m_lightSapceMatrix = lightProjection * lightView;
}

glm::mat4 ShadowMap::getLightSpaceMatrix() const { return m_lightSapceMatrix; }

}  // namespace te
