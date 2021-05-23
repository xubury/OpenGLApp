#ifndef MATH_HPP
#define MATH_HPP

#include <glm/glm.hpp>
#include <Entity/Camera.hpp>

inline glm::vec3 findClosestPoint(const glm::vec3 point, const glm::vec3 &start,
                                  const glm::vec3 &end) {
    glm::vec3 c = point - start;
    glm::vec3 v = end - start;
    float d = (end - start).length();
    v /= d;
    float t = glm::dot(v, c);
    if (t < std::numeric_limits<float>::epsilon()) {
        return start;
    } else if (t - d > std::numeric_limits<float>::epsilon()) {
        return end;
    }
    return start + v * t;
}

inline glm::vec4 buildPlane(const glm::vec3 &point, const glm::vec3 &normal) {
    return glm::vec4(normal, glm::dot(normal, point));
}

inline float intersectRayPlane(const glm::vec3 &rayStart, const glm::vec3 &dir,
                               glm::vec4 &plane) {
    glm::vec3 normal = plane;
    float numer = glm::dot(normal, rayStart) - plane.w;
    float denom = glm::dot(normal, dir);
    // orthogonal, can't intercect
    if (std::fabs(denom) < std::numeric_limits<float>::epsilon()) {
        return -1.0f;
    }

    return -numer / denom;
}

inline glm::vec3 computeWorldToSrceen(const glm::vec3 &worldPos,
                                      const Camera &camera) {
    glm::mat4 projectionView = camera.getProjection() * camera.getView();
    glm::vec4 clipPos = projectionView * glm::vec4(worldPos, 1.0f);
    clipPos /= clipPos.w;

    float width = camera.getWidth();
    float height = camera.getHeight();
    float x = camera.getX();
    float y = camera.getY();

    glm::vec3 screenPos;
    screenPos.x = (clipPos.x + 1) * 0.5 * width + x;
    screenPos.y = (1 - clipPos.y) * 0.5 * height + y;
    screenPos.z = clipPos.z;
    return screenPos;
}

inline void computeCameraRay(glm::vec3 &rayOrigin, glm::vec3 &rayDir,
                             const glm::vec2 &screenPos, const Camera &camera) {
    glm::vec2 mouseClipPos;
    mouseClipPos =
        (screenPos - camera.getViewportPos()) / camera.getViewportSize();
    mouseClipPos.x = mouseClipPos.x * 2.f - 1.f;
    mouseClipPos.y = (1.f - mouseClipPos.y) * 2.f - 1.f;
    const float zNear = 0.f;
    const float zFar = 1.f - std::numeric_limits<float>::epsilon();

    glm::mat4 inversePV =
        glm::inverse(camera.getProjection() * camera.getView());
    glm::vec4 rayOriginH = inversePV * glm::vec4(mouseClipPos, zNear, 1.0f);
    rayOriginH /= rayOriginH.w;
    rayOrigin = rayOriginH;

    glm::vec4 rayEnd = inversePV * glm::vec4(mouseClipPos, zFar, 1.0f);
    rayEnd /= rayEnd.w;

    rayDir = glm::normalize(glm::vec3(rayEnd) - rayOrigin);
}

#endif /* MATH_HPP */
