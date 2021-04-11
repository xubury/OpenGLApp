#include <glad/glad.h>

#include "VertexBuffer.hpp"
#include "Vertex.hpp"
#include "RenderTarget.hpp"

VertexBuffer::VertexBuffer() : m_VBO(0), m_size(0) {}

std::size_t VertexBuffer::size() const { return m_size; }

bool VertexBuffer::isAvailable() const { return m_size > 0; }

bool VertexBuffer::update(const std::vector<Vertex> &vertices) {
    return update(vertices.data(), vertices.size());
}

bool VertexBuffer::update(const Vertex *vertices, std::size_t cnt) {
    if (m_VBO == 0) {
        glGenBuffers(1, &m_VBO);
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * cnt, vertices,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_size = cnt;
    return true;
}

void VertexBuffer::draw(RenderTarget &target) const { target.draw(*this); }

void VertexBuffer::bind(const VertexBuffer *vertexBuffer) {
    if (vertexBuffer != nullptr)
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_VBO);
    else
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}
