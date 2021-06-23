#include "Model/Mesh.hpp"
#include "Graphic/Renderer.hpp"
#include "Graphic/Shader.hpp"
#include "Graphic/OpenGL.hpp"

namespace te {

Mesh::Mesh(GLenum type, const std::vector<Vertex> &vertices,
           const std::vector<uint32_t> &indices, Ref<Material> textures)
    : m_vertices(vertices),
      m_indices(indices),
      m_textures(textures),
      m_type(type) {
    m_vertexArray = createRef<VertexArray>();
    Ref<VertexBuffer> vertexBuffer = createRef<VertexBuffer>(
        vertices.data(), vertices.size() * sizeof(Vertex));
    vertexBuffer->setLayout({{ShaderDataType::Float3, "aPos"},
                             {ShaderDataType::Float2, "aTexCoord"},
                             {ShaderDataType::Float3, "aNormal"},
                             {ShaderDataType::Int4, "aBoneIds"},
                             {ShaderDataType::Float4, "aWeights"}});
    m_vertexArray->addVertexBuffer(vertexBuffer);
    m_vertexArray->setIndexBuffer(
        createRef<IndexBuffer>(indices.data(), indices.size()));
}

void Mesh::draw(const Shader &shader, const glm::mat4 &transform) const {
    Renderer::submit(shader, *m_vertexArray, m_type, !m_indices.empty(),
                     transform, m_textures.get());
}

const Vertex *Mesh::getVertex() const { return m_vertices.data(); }

std::size_t Mesh::size() const { return m_vertices.size(); }

}  // namespace te
