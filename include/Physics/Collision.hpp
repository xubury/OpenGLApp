#ifndef COLLISION_HPP
#define COLLISION_HPP

#include "Physics/Collider.hpp"
#include "Physics/Manifold.hpp"

namespace te {

using CollisionTest = ContactManifold (*)(Collider *objA,
                                          Transformable *transformA,
                                          Collider *objB,
                                          Transformable *transformB);

class Collision {
   public:
    static ContactManifold collide(Collider *objA, Transformable *transformA,
                                   Collider *objB, Transformable *transformB);

   private:
    static ContactManifold collideTerrainSphere(Collider *objA,
                                                Transformable *transformA,
                                                Collider *objB,
                                                Transformable *transformB);

    static ContactManifold collideTerrainHull(Collider *objA,
                                              Transformable *transformA,
                                              Collider *objB,
                                              Transformable *transformB);

    static ContactManifold collideSpheres(Collider *objA,
                                          Transformable *transformA,
                                          Collider *objB,
                                          Transformable *transformB);

    static ContactManifold collideSphereHull(Collider *objA,
                                             Transformable *transformA,
                                             Collider *objB,
                                             Transformable *transformB);

    static ContactManifold collideHulls(Collider *objA,
                                        Transformable *transformA,
                                        Collider *objB,
                                        Transformable *transformB);

    static const CollisionTest
        collisionTable[Collider::Type::COLLIDIER_TYPE_COUNT]
                      [Collider::Type::COLLIDIER_TYPE_COUNT];
};

}  // namespace te

#endif
