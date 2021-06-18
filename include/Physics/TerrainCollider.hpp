#ifndef TEERAIN_COLLIDER_HPP
#define TEERAIN_COLLIDER_HPP

#include "Physics/Collider.hpp"

namespace te {

class TerrainCollider : public Collider {
   public:
    TerrainCollider(const std::vector<glm::vec3> &vertices,
                    const std::vector<glm::vec3> &normals, int vertexCount,
                    int gridSize);

    float height(const glm::vec3 &localPos) const;

    glm::vec3 normal(const glm::vec3 &localPos) const;

    glm::vec3 findFurthestPoint(const glm::vec3 &) const override {
        TE_CORE_ASSERT(
            false,
            "TerrainCollider is not supposed to call findFurthestPoint!");
        return glm::vec3(0);
    }

   public:
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
    int m_vertexCount;
    int m_gridSize;
};

}  // namespace te

#endif /* TEERAIN_COLLIDER_HPP */
