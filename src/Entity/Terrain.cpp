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
            m_vertices[vertexPointer].y = 0.f;
            m_vertices[vertexPointer].z = i * gridSize;
            m_normals[vertexPointer].x = 0.f;
            m_normals[vertexPointer].y = 1.0f;
            m_normals[vertexPointer].z = 0.f;
            texCoords[vertexPointer].x =
                (float)(j + halfSize) / (vertexCount - 1);
            texCoords[vertexPointer].y =
                (float)(i + halfSize) / (vertexCount - 1);
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
    add<TerrainCollider>(m_vertices, m_gridSize, m_vertexCount);
}

void Terrain::draw(const Shader &shader, const glm::mat4 &transform) const {
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Renderer::submit(shader, *m_terrain, GL_TRIANGLES, true,
                     getTransform() * transform, m_material.get());
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

void Terrain::computeNormal() {
    float offset = 0.3f;
    for (int i = 1; i < m_vertexCount - 1; ++i) {
        float z = (i - (m_vertexCount - 1) / 2.f) * m_gridSize;
        for (int j = 1; j < m_vertexCount - 1; ++j) {
            float x = (j - (m_vertexCount - 1) / 2.f) * m_gridSize;
            float hL = height(x - offset, z);
            float hR = height(x + offset, z);
            float hD = height(x, z - offset);
            float hU = height(x, z + offset);
            glm::vec3 &normal = m_normals[i * m_vertexCount + j];
            normal.x = hL - hR;
            normal.y = 2.0 * offset;
            normal.z = hD - hU;
            normal = normalize(normal);
        }
    }
}

};  // namespace te
