#include "Graphic/Mesh.hpp"
#include "Graphic/RenderTarget.hpp"
#include "Graphic/Shader.hpp"
#include "Graphic/OpenGL.hpp"

namespace te {

Mesh::Mesh(GLenum type, const std::vector<Vertex> &vertices,
           const std::vector<uint32_t> &indices, const TextureArray &textures)
    : m_vertices(vertices), m_indices(indices), m_textures(textures) {
    m_buffer.update(m_vertices.data(), m_vertices.size(), m_indices.data(),
                    m_indices.size(), type, GL_STATIC_DRAW);
}

void Mesh::draw(RenderTarget &target, RenderStates states) const {
    states.textures = &m_textures;
    m_buffer.draw(target, states);
}

const Vertex *Mesh::getVertex() const { return m_vertices.data(); }

std::size_t Mesh::size() const { return m_vertices.size(); }

}  // namespace te
