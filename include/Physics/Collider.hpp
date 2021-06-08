#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "Core/Base.hpp"
#include "ECS/ECS.hpp"
#include "Physics/CollisionObject.hpp"

#include <glm/glm.hpp>

namespace te {

class SphereCollider;

class HullCollider;

class Collider : public Component<Collider, EntityBase> {
   public:
    // find the furthest point in direction using dot product
    virtual glm::vec3 findFurthestPoint(const glm::vec3 &direction) const = 0;

    virtual ContactManifold testCollision(const Collider &collider) const = 0;

    virtual ContactManifold testCollision(
        const SphereCollider &sphere) const = 0;

    virtual ContactManifold testCollision(const HullCollider &plane) const = 0;
};

}  // namespace te

#endif /* COLLIDER_HPP */
