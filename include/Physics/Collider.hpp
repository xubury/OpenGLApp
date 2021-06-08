#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "Core/Base.hpp"
#include "ECS/ECS.hpp"
#include "Physics/CollisionObject.hpp"

#include <glm/glm.hpp>

namespace te {

struct ContactPoint {
    glm::vec3 position;
    float depth;
    ContactPoint() : position(0), depth(0) {}
};

struct ContactManifold {
    Ref<CollisionObject> objA;
    Ref<CollisionObject> objB;
    uint8_t pointCount;
    ContactPoint points[4];
    glm::vec3 normal;

    ContactManifold()
        : objA(nullptr), objB(nullptr), pointCount(0), points(), normal(0) {}

    bool hasCollision() const { return pointCount > 0; }
};

class SphereCollider;

class HullCollider;

class Collider : public Component<Collider, EntityBase> {
   public:
    virtual glm::vec3 findFurthestPoint(const glm::vec3 &direction) const = 0;

    virtual ContactManifold testCollision(const Collider &collider) const = 0;

    virtual ContactManifold testCollision(
        const SphereCollider &sphere) const = 0;

    virtual ContactManifold testCollision(const HullCollider &plane) const = 0;
};

}  // namespace te

#endif /* COLLIDER_HPP */
