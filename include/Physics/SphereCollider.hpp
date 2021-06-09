#ifndef SPHER_COLLIDER_HPP
#define SPHER_COLLIDER_HPP

#include "Physics/Collider.hpp"
#include "Component/Transform.hpp"

namespace te {

class SphereCollider : public Collider {
   public:
    SphereCollider(const glm::vec3& center, float radius);

    glm::vec3 findFurthestPoint(const glm::vec3& direction) const override;

    ContactManifold testCollision(Collider& collider) override;

    ContactManifold testCollision(SphereCollider& sphere) override;

    ContactManifold testCollision(HullCollider&) override;

   private:
    glm::vec3 m_center;
    float m_radius;
};

}  // namespace te

#endif /* SPHER_COLLIDER_HPP */
