#include "Graphic/Camera.hpp"
#include "Core/Assert.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace te {

Camera::Camera(int x, int y, int width, int height)
    : m_viewportX(x),
      m_viewportY(y),
      m_viewportWidth(width),
      m_viewportHeight(height),
      m_nearZ(0.1),
      m_farZ(100.f),
      m_zoom(45.f) {
    TE_CORE_ASSERT(width > 0 && height > 0, "Camera width or height <= 0.");
}

glm::mat4 Camera::getView() const {
    const glm::vec3 &up = getUp();
    const glm::vec3 &front = getFront();
    const glm::vec3 &pos = getPosition();
    return glm::lookAt(pos, pos - front, up);
}

glm::mat4 Camera::getProjection() const {
    return glm::perspective(glm::radians(getFOV()), getAspect(), getNearZ(),
                            getFarZ());
}

int Camera::getViewportX() const { return m_viewportX; }

int Camera::getViewportY() const { return m_viewportY; }

glm::i32vec2 Camera::getViewportPos() const {
    return glm::vec2(m_viewportX, m_viewportY);
}

uint32_t Camera::getViewportWidth() const { return m_viewportWidth; }

uint32_t Camera::getViewportHeight() const { return m_viewportHeight; }

glm::u32vec2 Camera::getViewportSize() const {
    return glm::vec2(m_viewportWidth, m_viewportHeight);
}

float Camera::getNearZ() const { return m_nearZ; }

float Camera::getFarZ() const { return m_farZ; }

float Camera::getAspect() const {
    return (float)m_viewportWidth / m_viewportHeight;
}

void Camera::setViewportSize(float width, float height) {
    m_viewportWidth = width;
    m_viewportHeight = height;
}

void Camera::setNearFar(float near, float far) {
    m_nearZ = near;
    m_farZ = far;
}

void Camera::computeCameraRay(glm::vec3 &rayOrigin, glm::vec3 &rayDir,
                                  const glm::vec2 &screenPos) const {
    glm::vec2 mouseClipPos((screenPos.x - m_viewportX) / m_viewportWidth,
                           (screenPos.y + m_viewportY) / m_viewportHeight);
    mouseClipPos.x = mouseClipPos.x * 2.f - 1.f;
    mouseClipPos.y = (1.f - mouseClipPos.y) * 2.f - 1.f;
    const float zNear = 0.f;
    const float zFar = 1.f - std::numeric_limits<float>::epsilon();

    glm::mat4 inversePV = glm::inverse(getProjection() * getView());
    glm::vec4 rayOriginH = inversePV * glm::vec4(mouseClipPos, zNear, 1.0f);
    rayOriginH /= rayOriginH.w;
    rayOrigin = rayOriginH;

    glm::vec4 rayEnd = inversePV * glm::vec4(mouseClipPos, zFar, 1.0f);
    rayEnd /= rayEnd.w;

    rayDir = glm::normalize(glm::vec3(rayEnd) - rayOrigin);
}

glm::vec3 Camera::computeWorldToSrceen(const glm::vec3 &worldPos) const {
    glm::mat4 projectionView = getProjection() * getView();
    glm::vec4 clipPos = projectionView * glm::vec4(worldPos, 1.0f);
    clipPos /= clipPos.w;

    glm::vec3 screenPos;
    screenPos.x = (clipPos.x + 1) * 0.5 * m_viewportWidth + m_viewportX;
    screenPos.y = (1 - clipPos.y) * 0.5 * m_viewportHeight - m_viewportY;
    screenPos.z = clipPos.z;
    return screenPos;
}

float Camera::getSegmentLengthClipSpace(const glm::vec3 &start,
                                            const glm::vec3 &end) const {
    glm::mat4 projectionView = getProjection() * getView();
    glm::vec4 segStart = projectionView * glm::vec4(start, 1.0f);
    if (std::fabs(segStart.w) > std::numeric_limits<float>::epsilon()) {
        segStart /= segStart.w;
    }

    glm::vec4 segEnd = projectionView * glm::vec4(end, 1.0f);
    if (std::fabs(segEnd.w) > std::numeric_limits<float>::epsilon()) {
        segEnd /= segEnd.w;
    }

    glm::vec2 clipSpaceAxis = segEnd - segStart;
    clipSpaceAxis.y /= getAspect();
    return glm::length(clipSpaceAxis);
}

float Camera::getFOV() const { return m_zoom; }

}  // namespace te
