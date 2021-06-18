#ifndef COLLIDER_HPP
#define COLLIDER_HPP

#include "ECS/ECS.hpp"

#include <glm/glm.hpp>

namespace te {

class Collider : public Component<Collider, EntityBase> {
   public:
    enum Type {
        TEERAIN_COLLIDER,
        SPHERE_COLLIDER,
        HULL_COLLIDER,
        CAPSULE_COLLIDER,
        MESH_COLLIDER,
        COLLIDIER_TYPE_COUNT
    };

    Collider(Type type) : m_type(type) {}

    // find the furthest point in direction using dot product
    virtual glm::vec3 findFurthestPoint(const glm::vec3 &direction) const = 0;

    virtual Type getType() const { return m_type; };

   private:
    Type m_type;
};

}  // namespace te

#endif /* COLLIDER_HPP */
