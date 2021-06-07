#include "Physics/HullCollider.hpp"

namespace te {

ContactManifold HullCollider::testCollision(const Collider& collider) const {
    return collider.testCollision(*this);
}

ContactManifold HullCollider::testCollision(const SphereCollider&) const {
    return ContactManifold();
}

ContactManifold HullCollider::testCollision(const HullCollider&) const {
    return ContactManifold();
}

}  // namespace te
