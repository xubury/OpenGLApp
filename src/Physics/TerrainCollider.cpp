#include "Physics/TerrainCollider.hpp"
#include "Entity/Terrain.hpp"
#include "Core/Math.hpp"

namespace te {

TerrainCollider::TerrainCollider(const std::vector<glm::vec3> &vertices,
                                 uint32_t gridSizes, uint32_t vertexCount)
    : Collider(Collider::TERRAIN_COLLIDER),
      m_vertices(vertices),
      m_gridSize(gridSizes),
      m_vertexCount(vertexCount) {}

float TerrainCollider::height(float localX, float localZ) const {
    if (outOfBound(localX, localZ)) {
        return std::numeric_limits<float>::lowest();
    }
    float x = localX / m_gridSize + (m_vertexCount - 1) * 0.5;
    float z = localZ / m_gridSize + (m_vertexCount - 1) * 0.5;
    int x0 = std::floor(x);
    int z0 = std::floor(z);
    float f00 = m_vertices[z0 * m_vertexCount + x0].y;
    float f10 = m_vertices[z0 * m_vertexCount + (x0 + 1)].y;
    float f01 = m_vertices[(z0 + 1) * m_vertexCount + x0].y;
    float f11 = m_vertices[(z0 + 1) * m_vertexCount + (x0 + 1)].y;
    float xCoord = x - x0;
    float zCoord = z - z0;
    if (xCoord <= 1 - zCoord) {
        return baryCentric(glm::vec3(0, f00, 0), glm::vec3(1, f10, 0),
                           glm::vec3(0, f01, 1), glm::vec2(xCoord, zCoord));
    } else {
        return baryCentric(glm::vec3(1, f10, 0), glm::vec3(1, f11, 1),
                           glm::vec3(0, f01, 1), glm::vec2(xCoord, zCoord));
    }
}

glm::vec3 TerrainCollider::normal(float localX, float localZ) const {
    float offset = 0.3f;
    float hL = height(localX - offset, localZ);
    float hR = height(localX + offset, localZ);
    float hD = height(localX, localZ - offset);
    float hU = height(localX, localZ + offset);
    glm::vec3 normal =
        glm::normalize(glm::vec3(hL - hR, 2.0f * offset, hD - hU));
    return normal;
}

bool TerrainCollider::outOfBound(float localX, float localZ) const {
    float x = (localX / m_gridSize + (m_vertexCount - 1) * 0.5f) /
              (m_vertexCount - 1);
    float z = (localZ / m_gridSize + (m_vertexCount - 1) * 0.5f) /
              (m_vertexCount - 1);
    return std::isnan(localX) || std::isnan(localZ) || x <= 0 || z <= 0 ||
           x >= 1 || z >= 1;
}

}  // namespace te
