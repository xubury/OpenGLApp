#ifndef CAMERA_BASE_HPP
#define CAMERA_BASE_HPP

#include "Graphic/Export.hpp"
#include <glm/glm.hpp>

class GRAPHIC_API CameraBase {
   public:
    CameraBase(int x, int y, int width, int height);

    virtual glm::mat4 getProjection() const = 0;

    virtual glm::mat4 getView() const = 0;

    int getViewportX() const;

    int getViewportY() const;

    glm::vec2 getViewportPos() const;

    int getViewportWidth() const;

    int getViewportHeight() const;

    glm::vec2 getViewportSize() const;

    void setViewportSize(float width, float height);

    float getNearZ() const;

    float getFarZ() const;

    float getAspect() const;

    void setNearFar(float near, float far);

    void computeCameraRay(glm::vec3 &rayOrigin, glm::vec3 &rayDir,
                          const glm::vec2 &screenPos) const;

    glm::vec3 computeWorldToSrceen(const glm::vec3 &worldPos) const;

    float getSegmentLengthClipSpace(const glm::vec3 &start,
                                    const glm::vec3 &end) const;

   private:
    int m_viewportX;
    int m_viewportY;
    int m_viewportWidth;
    int m_viewportHeight;

    float m_nearZ;
    float m_farZ;
};

#endif /* CAMERA_BASE_HPP */
