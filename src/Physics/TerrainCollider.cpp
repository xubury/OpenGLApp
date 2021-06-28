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

    float u, v, w;
    if (xCoord <= 1 - zCoord) {
        glm::vec2 a(0, 0);
        glm::vec2 b(1, 0);
        glm::vec2 c(0, 1);
        baryCentric(a, b, c, glm::vec2(xCoord, zCoord), u, v, w);
        return f00 * u + f10 * v + f01 * w;
    } else {
        glm::vec2 a(1, 0);
        glm::vec2 b(1, 1);
        glm::vec2 c(0, 1);
        baryCentric(a, b, c, glm::vec2(xCoord, zCoord), u, v, w);
        return f10 * u + f11 * v + f01 * w;
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
