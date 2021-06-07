#include "Graphic/CameraBase.hpp"
#include "Core/Assert.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace te {

CameraBase::CameraBase(int x, int y, int width, int height)
    : m_viewportX(x),
      m_viewportY(y),
      m_viewportWidth(width),
      m_viewportHeight(height),
      m_nearZ(0.1),
      m_farZ(100.f),
      m_primary(false) {
    TE_CORE_ASSERT(width > 0 && height > 0, "CameraBase width or height <= 0.");
}

int CameraBase::getViewportX() const { return m_viewportX; }

int CameraBase::getViewportY() const { return m_viewportY; }

glm::vec2 CameraBase::getViewportPos() const {
    return glm::vec2(m_viewportX, m_viewportY);
}

int CameraBase::getViewportWidth() const { return m_viewportWidth; }

int CameraBase::getViewportHeight() const { return m_viewportHeight; }

glm::vec2 CameraBase::getViewportSize() const {
    return glm::vec2(m_viewportWidth, m_viewportHeight);
}

float CameraBase::getNearZ() const { return m_nearZ; }

float CameraBase::getFarZ() const { return m_farZ; }

float CameraBase::getAspect() const {
    return (float)m_viewportWidth / m_viewportHeight;
}

void CameraBase::setViewportSize(float width, float height) {
    m_viewportWidth = width;
    m_viewportHeight = height;
}

void CameraBase::setNearFar(float near, float far) {
    m_nearZ = near;
    m_farZ = far;
}

void CameraBase::computeCameraRay(glm::vec3 &rayOrigin, glm::vec3 &rayDir,
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

glm::vec3 CameraBase::computeWorldToSrceen(const glm::vec3 &worldPos) const {
    glm::mat4 projectionView = getProjection() * getView();
    glm::vec4 clipPos = projectionView * glm::vec4(worldPos, 1.0f);
    clipPos /= clipPos.w;

    glm::vec3 screenPos;
    screenPos.x = (clipPos.x + 1) * 0.5 * m_viewportWidth + m_viewportX;
    screenPos.y = (1 - clipPos.y) * 0.5 * m_viewportHeight - m_viewportY;
    screenPos.z = clipPos.z;
    return screenPos;
}

float CameraBase::getSegmentLengthClipSpace(const glm::vec3 &start,
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

}  // namespace te
