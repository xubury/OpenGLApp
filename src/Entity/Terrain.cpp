#include "Entity/Terrain.hpp"
#include "Graphic/Renderer.hpp"
#include "Physics/TerrainCollider.hpp"
#include "Physics/Rigidbody.hpp"
#include "Core/Math.hpp"

namespace te {

Terrain::Terrain(EntityManager<EntityBase> *manager, uint32_t id, int gridSize,
                 int vertexCount, Ref<Material> material)
    : EntityBase(manager, id),
      m_material(material),
      m_gridSize(gridSize),
      m_vertexCount(vertexCount) {
    m_vertices.resize(vertexCount * vertexCount);
    m_normals.resize(vertexCount * vertexCount);
    std::vector<glm::vec2> texCoords(vertexCount * vertexCount);
    std::vector<uint32_t> indices(6 * (vertexCount - 1) * (vertexCount - 1));
    int vertexPointer = 0;
    float halfSize = (vertexCount - 1) * 0.5f;
    for (float i = -halfSize; i <= halfSize; ++i) {
        for (float j = -halfSize; j <= halfSize; ++j) {
            RandomGenerator random(vertexPointer);
            m_vertices[vertexPointer].x = j * gridSize;
            m_vertices[vertexPointer].y = random.rnd(0.1f, 4.f);
            m_vertices[vertexPointer].z = i * gridSize;
            m_normals[vertexPointer].x = 0.f;
            m_normals[vertexPointer].y = 1.0f;
            m_normals[vertexPointer].z = 0.f;
            texCoords[vertexPointer].x = (float)j / (vertexCount - 1);
            texCoords[vertexPointer].y = (float)i / (vertexCount - 1);
            vertexPointer++;
        }
    }
    computeNormal();
    int indexPointer = 0;
    for (int gz = 0; gz < vertexCount - 1; gz++) {
        for (int gx = 0; gx < vertexCount - 1; gx++) {
            int topLeft = (gz * vertexCount) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz + 1) * vertexCount) + gx;
            int bottomRight = bottomLeft + 1;
            indices[indexPointer++] = topLeft;
            indices[indexPointer++] = bottomLeft;
            indices[indexPointer++] = topRight;
            indices[indexPointer++] = topRight;
            indices[indexPointer++] = bottomLeft;
            indices[indexPointer++] = bottomRight;
        }
    }
    m_terrain = createRef<VertexArray>();

    Ref<VertexBuffer> buffer = createRef<VertexBuffer>(
        m_vertices.data(), m_vertices.size() * sizeof(glm::vec3));
    buffer->setLayout({{ShaderDataType::Float3, "aPos"}});
    m_terrain->addVertexBuffer(buffer);

    buffer = createRef<VertexBuffer>(texCoords.data(),
                                     texCoords.size() * sizeof(glm::vec2));
    buffer->setLayout({{ShaderDataType::Float2, "aTexCoord"}});
    m_terrain->addVertexBuffer(buffer);

    buffer = createRef<VertexBuffer>(m_normals.data(),
                                     m_normals.size() * sizeof(glm::vec3));
    buffer->setLayout({{ShaderDataType::Float3, "aNormal"}});
    m_terrain->addVertexBuffer(buffer);

    m_terrain->setIndexBuffer(
        createRef<IndexBuffer>(indices.data(), indices.size()));
    add<Rigidbody>(10, false);
    add<TerrainCollider>();
}

void Terrain::draw(const Shader &shader) const {
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Renderer::submit(shader, *m_terrain, GL_TRIANGLES, true, getTransform(),
                     m_material.get());
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

float Terrain::height(float localX, float localZ) const {
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

glm::vec3 Terrain::normal(float localX, float localZ) const {
    // FIXME: not correct here
    float offset = m_gridSize / 2.f;
    float hL = height(localX - offset, localZ);
    float hR = height(localX + offset, localZ);
    float hD = height(localX, localZ - offset);
    float hU = height(localX, localZ + offset);
    glm::vec3 normal =
        glm::normalize(glm::vec3(hL - hR, 2.0f * offset, hD - hU));
    normal = glm::vec3(0, 1, 0);
    return normal;
}

bool Terrain::outOfBound(const glm::vec3 &localPos) const {
    float x = (localPos.x / m_gridSize + (m_vertexCount - 1) * 0.5f) /
              (m_vertexCount - 1);
    float z = (localPos.z / m_gridSize + (m_vertexCount - 1) * 0.5f) /
              (m_vertexCount - 1);
    return std::isnan(localPos.x) || std::isnan(localPos.y) ||
           std::isnan(localPos.z) || x <= 0 || z <= 0 || x >= 1 || z >= 1;
}

void Terrain::computeNormal() {
    for (int i = 1; i < m_vertexCount - 1; ++i) {
        for (int j = 1; j < m_vertexCount - 1; ++j) {
            float hL = m_vertices[i * m_vertexCount + j - 1].y;
            float hR = m_vertices[i * m_vertexCount + j + 1].y;
            float hD = m_vertices[(i - 1) * m_vertexCount + j].y;
            float hU = m_vertices[(i + 1) * m_vertexCount + j].y;
            glm::vec3 &normal = m_normals[i * m_vertexCount + j];
            normal.x = hL - hR;
            normal.z = hD - hU;
            normal.y = 2.0 * m_gridSize;
            normal = normalize(normal);
        }
    }
}

};  // namespace te
