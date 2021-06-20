#include "Physics/TerrainCollider.hpp"
#include "Entity/Terrain.hpp"

namespace te {

TerrainCollider::TerrainCollider() : Collider(Collider::TEERAIN_COLLIDER) {}

float TerrainCollider::height(float localX, float localZ) const {
    return dynamic_cast<const Terrain *>(owner())->height(localX, localZ);
}

glm::vec3 TerrainCollider::normal(const glm::vec3 &localPos) const {
    return dynamic_cast<const Terrain *>(owner())->normal(localPos);
}

bool TerrainCollider::outOfBound(const glm::vec3 &localPos) const {
    return dynamic_cast<const Terrain *>(owner())->outOfBound(localPos);
}

}  // namespace te
