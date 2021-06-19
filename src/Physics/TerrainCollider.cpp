#include "Physics/TerrainCollider.hpp"
#include "Entity/Terrain.hpp"

namespace te {

TerrainCollider::TerrainCollider() : Collider(Collider::TEERAIN_COLLIDER) {}

float TerrainCollider::height(const glm::vec3 &localPos) const {
    return dynamic_cast<const Terrain *>(owner())->height(localPos);
}

glm::vec3 TerrainCollider::normal(const glm::vec3 &localPos) const {
    return dynamic_cast<const Terrain *>(owner())->normal(localPos);
}

}  // namespace te
