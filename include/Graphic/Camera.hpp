#ifndef CAMERA_BASE_HPP
#define CAMERA_BASE_HPP

#include "Core/Export.hpp"
#include "Core/Transform.hpp"
#include <glm/glm.hpp>

namespace te {

class TE_API Camera : public Transformable {
   public:
    Camera(int x, int y, int width, int height);

    glm::mat4 getProjection() const;

    glm::mat4 getView() const;

    int getViewportX() const;

    int getViewportY() const;

    glm::i32vec2 getViewportPos() const;

    uint32_t getViewportWidth() const;

    uint32_t getViewportHeight() const;

    glm::u32vec2 getViewportSize() const;

    void setViewportSize(float width, float height);

    float getNearZ() const;

    float getFarZ() const;

    float getAspect() const;

    void setNearFar(float near, float far);

    float getFOV() const;

    void computeCameraRay(glm::vec3 &rayOrigin, glm::vec3 &rayDir,
                          const glm::vec2 &screenPos) const;

    glm::vec3 computeWorldToSrceen(const glm::vec3 &worldPos) const;

    float getSegmentLengthClipSpace(const glm::vec3 &start,
                                    const glm::vec3 &end) const;

   protected:
    int m_viewportX;
    int m_viewportY;
    uint32_t m_viewportWidth;
    uint32_t m_viewportHeight;

    float m_nearZ;
    float m_farZ;
    float m_zoom;
};

}  // namespace te

#endif /* CAMERA_BASE_HPP */
