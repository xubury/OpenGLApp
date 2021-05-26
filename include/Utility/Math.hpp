#ifndef MATH_HPP
#define MATH_HPP

#include <glm/glm.hpp>
#include <Entity/Camera.hpp>

template <glm::length_t L, typename T, glm::qualifier Q>
inline glm::vec<L, T, Q> findClosestPoint(const glm::vec<L, T, Q> point,
                                          const glm::vec<L, T, Q> &start,
                                          const glm::vec<L, T, Q> &end) {
    glm::vec<L, T, Q> c = point - start;
    glm::vec<L, T, Q> v = end - start;
    T d = glm::length(end - start);
    v /= d;
    float t = glm::dot(v, c);
    if (t < 0) {
        return start;
    } else if (t > d) {
        return end;
    }
    return start + v * t;
}

inline glm::vec4 buildPlane(const glm::vec3 &point, const glm::vec3 &normal) {
    return glm::vec4(normal, glm::dot(normal, point));
}

inline float intersectRayPlane(const glm::vec3 &rayOrigin, const glm::vec3 &dir,
                               const glm::vec4 &plane) {
    glm::vec3 normal = plane;
    float numer = glm::dot(normal, rayOrigin) - plane.w;
    float denom = glm::dot(normal, dir);

    // orthogonal, can't intercect
    if (std::fabs(denom) < std::numeric_limits<float>::epsilon()) {
        return -1.0f;
    }

    return -numer / denom;
}

#endif /* MATH_HPP */
