#ifndef MANIFOLD_HPP
#define MANIFOLD_HPP

#include "CollisionObject.hpp"

namespace te {

struct ContactPoint {
    glm::vec3 positionA;
    glm::vec3 positionB;
    glm::vec3 normal;
    float normalImpusleSum;
    float tangentImpusleSum1;
    float tangentImpusleSum2;
    ContactPoint()
        : normal(0.f),
          normalImpusleSum(0.f),
          tangentImpusleSum1(0.f),
          tangentImpusleSum2(0.f) {}
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
