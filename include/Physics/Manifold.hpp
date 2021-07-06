#ifndef MANIFOLD_HPP
#define MANIFOLD_HPP

#include "CollisionObject.hpp"

namespace te {

struct ContactPoint {
    glm::vec3 positionA;
    glm::vec3 positionB;
    glm::vec3 normal;
    glm::vec3 tangent1;
    glm::vec3 tangent2;
    float normalImpusleSum;
    float tangentImpusleSum1;
    float tangentImpusleSum2;

    float depth;

    float bias;
    ContactPoint()
        : normalImpusleSum(0.f),
          tangentImpusleSum1(0.f),
          tangentImpusleSum2(0.f),
          depth(0.f),
          bias(0.f) {}
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
