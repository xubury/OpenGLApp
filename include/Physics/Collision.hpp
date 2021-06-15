#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Physics/Collider.hpp"
#include "Physics/Manifold.hpp"

namespace te {

using CollisionTest = ContactManifold (*)(Collider &objA, Collider &objB);

class Collision {
   public:
    static ContactManifold collide(Collider &objA, Collider &objB);

   private:
    static ContactManifold collideSpheres(Collider &objA, Collider &objB);

    static ContactManifold collideSphereHull(Collider &objA, Collider &objB);

    static ContactManifold collideHulls(Collider &objA, Collider &objB);

    static const CollisionTest
        collisionTable[Collider::Type::COLLIDIER_TYPE_COUNT]
                      [Collider::Type::COLLIDIER_TYPE_COUNT];
};

}  // namespace te

#endif
