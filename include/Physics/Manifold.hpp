#ifndef MANIFOLD_HPP
#define MANIFOLD_HPP

#include "CollisionObject.hpp"

namespace te {

struct ContactPoint {
    glm::vec3 position;
    glm::vec3 positionA;
    glm::vec3 positionB;
    float depth;
    ContactPoint() : position(0), depth(0) {}
};

struct ContactManifold {
    CollisionObject *objA;
    CollisionObject *objB;
    uint8_t pointCount;
    ContactPoint points[4];
    glm::vec3 normal;

    ContactManifold()
        : objA(nullptr), objB(nullptr), pointCount(0), points(), normal(0) {}

    bool hasCollision() const { return pointCount > 0; }
};

}  // namespace te

#endif
