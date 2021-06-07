#ifndef HULL_COLLIDER_HPP
#define HULL_COLLIDER_HPP

#include "Physics/Collider.hpp"

namespace te {

class HullCollider : public Collider {
   public:
    HullCollider() = default;

    ContactManifold testCollision(const Collider& collider) const override;

    ContactManifold testCollision(const SphereCollider&) const override;

    ContactManifold testCollision(const HullCollider&) const override;
};

}  // namespace te

#endif /* HULL_COLLIDER_HPP */
