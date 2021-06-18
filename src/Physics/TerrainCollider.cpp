#include "Physics/TerrainCollider.hpp"

namespace te {
TerrainCollider::TerrainCollider(const std::vector<glm::vec3> &vertices,
                                 const std::vector<glm::vec3> &normals,
                                 int vertexCount, int gridSize)
    : Collider(Collider::TEERAIN_COLLIDER),
      m_vertices(vertices),
      m_normals(normals),
      m_vertexCount(vertexCount),
      m_gridSize(gridSize) {}

float TerrainCollider::height(const glm::vec3 &localPos) const {
    int gridX = (localPos.x / m_gridSize + 0.5) * (m_vertexCount - 1);
    int gridZ = (localPos.z / m_gridSize + 0.5) * (m_vertexCount - 1);
    return m_vertices[gridZ * m_vertexCount + gridX].y;
}

glm::vec3 TerrainCollider::normal(const glm::vec3 &localPos) const {
    int gridX = (localPos.x / m_gridSize + 0.5) * (m_vertexCount - 1);
    int gridZ = (localPos.z / m_gridSize + 0.5) * (m_vertexCount - 1);
    return m_normals[gridZ * m_vertexCount + gridX];
}

}  // namespace te
