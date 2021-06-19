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
    for (int i = 0; i < vertexCount; ++i) {
        for (int j = 0; j < vertexCount; ++j) {
            RandomGenerator random((i & 0xFFFF) << 16 | (j & 0xFFFF));
            m_vertices[vertexPointer].x =
                ((float)j / (vertexCount - 1) - 0.5f) * gridSize;
            m_vertices[vertexPointer].y = random.rnd(4.f, 6.f);
            m_vertices[vertexPointer].z =
                ((float)i / (vertexCount - 1) - 0.5f) * gridSize;
            m_normals[vertexPointer].x = 0;
            m_normals[vertexPointer].y = 1;
            m_normals[vertexPointer].z = 0;
            texCoords[vertexPointer].x = (float)j / (vertexCount - 1);
            texCoords[vertexPointer].y = (float)i / (vertexCount - 1);
            vertexPointer++;
        }
    }
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Renderer::submit(shader, *m_terrain, GL_TRIANGLES, true, getTransform(),
                     m_material.get());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

float Terrain::height(const glm::vec3 &localPos) const {
    float x = (localPos.x / m_gridSize + 0.5f) * (m_vertexCount - 1);
    float z = (localPos.z / m_gridSize + 0.5f) * (m_vertexCount - 1);
    int x1 = std::ceil(x);
    int z1 = std::ceil(z);
    int x0 = std::floor(x);
    int z0 = std::floor(z);
    float f00 = m_vertices[z0 * m_vertexCount + x0].y;
    float f10 = m_vertices[z0 * m_vertexCount + x1].y;
    float f01 = m_vertices[z1 * m_vertexCount + x0].y;
    float f11 = m_vertices[z1 * m_vertexCount + x1].y;
    float f1 = f00 * (x1 - x) * (z1 - z);
    float f2 = f10 * (x - x0) * (z1 - z);
    float f3 = f01 * (x1 - x) * (z - z0);
    float f4 = f11 * (x - x0) * (z - z0);
    return f1 + f2 + f3 + f4;
}

glm::vec3 Terrain::normal(const glm::vec3 &localPos) const {
    int gridX = (localPos.x / m_gridSize + 0.5) * (m_vertexCount - 1);
    int gridZ = (localPos.z / m_gridSize + 0.5) * (m_vertexCount - 1);
    return m_normals[gridZ * m_vertexCount + gridX];
}

};  // namespace te
