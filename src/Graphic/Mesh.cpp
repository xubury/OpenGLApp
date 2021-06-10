#include "Graphic/Mesh.hpp"
#include "Graphic/Shader.hpp"
#include "Graphic/OpenGL.hpp"

namespace te {

Mesh::Mesh(GLenum type, const std::vector<Vertex> &vertices,
           const std::vector<uint32_t> &indices, const TextureArray &textures)
    : m_vertices(vertices), m_indices(indices), m_textures(textures) {
    m_vertexArray = createRef<VertexArray>(type);
    Ref<VertexBuffer> vertexBuffer = createRef<VertexBuffer>(
        vertices.data(), vertices.size() * sizeof(Vertex));
    vertexBuffer->setLayout({{ShaderDataType::Float3, "aPos"},
                             {ShaderDataType::Float2, "aTexCoord"},
                             {ShaderDataType::Float3, "aNormal"}});
    m_vertexArray->addVertexBuffer(vertexBuffer);
    m_vertexArray->setIndexBuffer(
        createRef<IndexBuffer>(indices.data(), indices.size()));
}

void Mesh::draw(const Ref<Shader> &shader, const glm::mat4 &transform) const {
    Renderer::submit(shader, m_vertexArray, transform);
}

const Vertex *Mesh::getVertex() const { return m_vertices.data(); }

std::size_t Mesh::size() const { return m_vertices.size(); }

}  // namespace te
