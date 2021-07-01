#ifndef TERRAIN_COLLIDER_HPP
#define TERRAIN_COLLIDER_HPP

#include "Physics/Collider.hpp"

namespace te {

class TerrainCollider : public Collider {
   public:
    TerrainCollider(const std::vector<glm::vec3> &vertices, uint32_t gridSizes,
                    uint32_t vertexCount);

    float height(float localX, float localZ) const;

    glm::vec3 normal(float localX, float localZ) const;

    bool outOfBound(float localX, float localZ) const;

    glm::vec3 findFurthestPoint(const glm::vec3 &,
                                const Transformable &) const override {
        TE_CORE_ASSERT(
            false,
            "TerrainCollider is not supposed to call findFurthestPoint!");
        return glm::vec3(0);
    }

   private:
    std::vector<glm::vec3> m_vertices;
    uint32_t m_gridSize;
    uint32_t m_vertexCount;
};

}  // namespace te

#endif /* TERRAIN_COLLIDER_HPP */
