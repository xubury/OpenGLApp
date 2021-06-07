#ifndef SPHER_COLLIDER_HPP
#define SPHER_COLLIDER_HPP

#include "Physics/Collider.hpp"
#include "Component/Transform.hpp"

namespace te {

class SphereCollider : public Collider {
   public:
    SphereCollider(const glm::vec3& center, float radius);

    ContactManifold testCollision(const Collider& collider) const override;

    ContactManifold testCollision(const SphereCollider& sphere) const override;

    ContactManifold testCollision(const HullCollider&) const override;

   private:
    glm::vec3 m_center;
    float m_radius;
};

}  // namespace te

#endif /* SPHER_COLLIDER_HPP */
