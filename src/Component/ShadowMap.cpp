#include "Component/ShadowMap.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "Core/Log.hpp"

namespace te {

ShadowMap::ShadowMap(float dist) : m_shadowDist(dist) {}

void ShadowMap::computeLightSpaceMatrix(const Camera &camera) {
    const glm::mat3 &rotation = owner()->getTransform();
    const glm::mat3 &rotaionInv = glm::transpose(rotation);
    const glm::mat3 &transform = rotaionInv * glm::mat3(camera.getTransform());
    const float nearZ = -10.f;
    const float farZ = 50.f;
    const float fov = camera.getFOV() / 2.f;
    const float aspect = camera.getAspect();
    const float nearHeight = nearZ * std::tan(glm::radians(fov));
    const float nearWidth = nearHeight * aspect;
    const float farHeight = farZ * std::tan(glm::radians(fov));
    const float farWidth = farHeight * aspect;
    const glm::vec3 &camPos = rotaionInv * camera.getPosition();
    std::vector<glm::vec3> vertices(8);
    vertices[0] =
        transform * glm::vec4(-nearWidth, -nearHeight, -nearZ, 1) + camPos;
    vertices[1] =
        transform * glm::vec4(-nearWidth, nearHeight, -nearZ, 1) + camPos;
    vertices[2] =
        transform * glm::vec4(nearWidth, -nearHeight, -nearZ, 1) + camPos;
    vertices[3] =
        transform * glm::vec4(nearWidth, nearHeight, -nearZ, 1) + camPos;

    vertices[4] =
        transform * glm::vec4(-farWidth, -farHeight, -farZ, 1) + camPos;
    vertices[5] =
        transform * glm::vec4(-farWidth, farHeight, -farZ, 1) + camPos;
    vertices[6] =
        transform * glm::vec4(farWidth, -farHeight, -farZ, 1) + camPos;
    vertices[7] = transform * glm::vec4(farWidth, farHeight, -farZ, 1) + camPos;

    glm::vec3 min(std::numeric_limits<float>::max());
    glm::vec3 max(std::numeric_limits<float>::lowest());
    for (int i = 0; i < 8; ++i) {
        min.x = std::min(min.x, vertices[i].x);
        min.y = std::min(min.y, vertices[i].y);
        min.z = std::min(min.z, vertices[i].z);
        max.x = std::max(max.x, vertices[i].x);
        max.y = std::max(max.y, vertices[i].y);
        max.z = std::max(max.z, vertices[i].z);
    }
    // Add a offset for shadow behind the camera frustum
    min.z -= 10.f;
    glm::vec3 size = max - min;
    glm::vec3 center = (max + min) / 2.f;
    center.z -= size.z / 2.f;
    glm::vec3 pos = rotation * center;
    const glm::vec3 &up = owner()->getUp();
    const glm::vec3 &front = owner()->getFront();
    glm::mat4 lightView = glm::lookAt(pos, pos + front, up);
    glm::mat4 lightProjection = glm::ortho(
        -size.x / 2.f, size.x / 2.f, -size.y / 2.f, size.y / 2.f, 0.f, size.z);
    m_lightSapceMatrix = lightProjection * lightView;
}

glm::mat4 ShadowMap::getLightSpaceMatrix() const { return m_lightSapceMatrix; }

}  // namespace te
