#ifndef MANIFOLD_HPP
#define MANIFOLD_HPP

#include "CollisionObject.hpp"

namespace te {

struct ContactPoint {
    glm::vec3 position;
    glm::vec3 positionA;
    glm::vec3 positionB;
    glm::vec3 normal;
    float depth;
    ContactPoint() : position(0.f), normal(0.f), depth(0.f) {}
};

struct ContactManifold {
    CollisionObject *objA;
    CollisionObject *objB;
    uint8_t pointCount;
    ContactPoint points[4];

    ContactManifold() : objA(nullptr), objB(nullptr), pointCount(0) {}

    bool hasCollision() const { return pointCount > 0; }
};

}  // namespace te

#endif
