#include "Entity/Terrain.hpp"
#include "Graphic/Vertex.hpp"
#include "Graphic/Renderer.hpp"

namespace te {

Terrain::Terrain(EntityManager<EntityBase> *manager, uint32_t id, int gridSize,
                 int vertexCount, Ref<Material> material)
    : EntityBase(manager, id), m_material(material) {
    int count = vertexCount * vertexCount;
    std::vector<Vertex> vertices(count);
    std::vector<uint32_t> indices(6 * (vertexCount - 1) * (vertexCount - 1));
    int vertexPointer = 0;
    for (int i = 0; i < vertexCount; ++i) {
        for (int j = 0; j < vertexCount; ++j) {
            vertices[vertexPointer].position.x =
                ((float)j / (vertexCount - 1) - 0.5f) * gridSize;
            vertices[vertexPointer].position.y = 0;
            vertices[vertexPointer].position.z =
                ((float)i / (vertexCount - 1) - 0.5f) * gridSize;
            vertices[vertexPointer].normal.x = 0;
            vertices[vertexPointer].normal.y = 1;
            vertices[vertexPointer].normal.z = 0;
            vertices[vertexPointer].texCoord.x = (float)j / (vertexCount - 1);
            vertices[vertexPointer].texCoord.y = (float)i / (vertexCount - 1);
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
        vertices.data(), vertices.size() * sizeof(Vertex));
    buffer->setLayout({{ShaderDataType::Float3, "aPos"},
                       {ShaderDataType::Float2, "aTexCoord"},
                       {ShaderDataType::Float3, "aNormal"}});
    m_terrain->addVertexBuffer(buffer);
    m_terrain->setIndexBuffer(
        createRef<IndexBuffer>(indices.data(), indices.size()));
}

void Terrain::draw(const Shader &shader) const {
    Renderer::submit(shader, *m_terrain, GL_TRIANGLES, true, getTransform(),
                     m_material.get());
}

};  // namespace te
