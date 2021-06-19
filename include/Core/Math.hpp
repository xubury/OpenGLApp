#ifndef MATH_HPP
#define MATH_HPP

#include "Core/Time.hpp"
#include <glm/glm.hpp>
#include <random>
#include "Core/Log.hpp"

namespace te {

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

inline bool sameDirection(const glm::vec3 &v1, const glm::vec3 &v2) {
    return glm::dot(v1, v2) > 0;
}

inline float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
                          glm::vec2 pos) {
    float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
    float l1 =
        ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
    float l2 =
        ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
    float l3 = 1.0f - l1 - l2;
    return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

class RandomGenerator {
   public:
    double rnd(double min, double max) {
        return ((double)lehmer64() / (double)(0x7FFFFFFF)) * (max - min) + min;
    }

    int rnd(int min, int max) { return (lehmer64() % (max - min)) + min; }

    RandomGenerator() {
        std::random_device rd;
        m_nProcGen = rd();
    }

    RandomGenerator(uint32_t seed) : m_nProcGen(seed) {}

   private:
    uint32_t m_nProcGen = 0;
    uint32_t lehmer64() {
        m_nProcGen += 0xe120fc15;
        uint64_t tmp;
        tmp = (uint64_t)m_nProcGen * 0x4a39b70d;
        uint32_t m1 = (tmp >> 32) ^ tmp;
        tmp = (uint64_t)m1 * 0x12fad5c9;
        uint32_t m2 = (tmp >> 32) ^ tmp;
        return m2;
    }
};

}  // namespace te

#endif /* MATH_HPP */
