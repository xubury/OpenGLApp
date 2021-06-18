#include "Entity/Terrain.hpp"
#include "Graphic/Renderer.hpp"
#include "Physics/TerrainCollider.hpp"
#include "Physics/Rigidbody.hpp"

namespace te {

Terrain::Terrain(EntityManager<EntityBase> *manager, uint32_t id, int gridSize,
                 int vertexCount, Ref<Material> material)
    : EntityBase(manager, id),
      m_material(material),
      m_vertexCount(vertexCount) {
    m_vertices.resize(vertexCount * vertexCount);
    m_normals.resize(vertexCount * vertexCount);
    std::vector<glm::vec2> texCoords(vertexCount * vertexCount);
    std::vector<uint32_t> indices(6 * (vertexCount - 1) * (vertexCount - 1));
    int vertexPointer = 0;
    for (int i = 0; i < vertexCount; ++i) {
        for (int j = 0; j < vertexCount; ++j) {
            m_vertices[vertexPointer].x =
                ((float)j / (vertexCount - 1) - 0.5f) * gridSize;
            m_vertices[vertexPointer].y = 0;
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
    add<TerrainCollider>(m_vertices, m_normals, vertexCount, gridSize);
}

void Terrain::draw(const Shader &shader) const {
    Renderer::submit(shader, *m_terrain, GL_TRIANGLES, true, getTransform(),
                     m_material.get());
}

};  // namespace te
