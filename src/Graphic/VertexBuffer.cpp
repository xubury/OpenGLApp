#include <glad/glad.h>
#include <iostream>

#include "Graphic/VertexBuffer.hpp"
#include "Graphic/Vertex.hpp"
#include "Graphic/RenderStates.hpp"
#include "Graphic/RenderTarget.hpp"

VertexBuffer::VertexBuffer() : m_VBO(0), m_size(0) {}

void VertexBuffer::destroy() { glDeleteBuffers(1, &m_VBO); }

std::size_t VertexBuffer::size() const { return m_size; }

bool VertexBuffer::empty() const { return m_size == 0; }

bool VertexBuffer::create(const Vertex *vertices, std::size_t cnt) {
    if (!m_VBO) glGenBuffers(1, &m_VBO);
    if (!m_VBO) {
        std::cerr << "Could not create vertex buffer." << std::endl;
        return false;
    }
    update(vertices, cnt);
    return true;
}

void VertexBuffer::update(const Vertex *vertices, std::size_t cnt) {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * cnt, vertices,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    m_size = cnt;
}

void VertexBuffer::draw(RenderTarget &target, RenderStates states) const {
    target.draw(*this, states);
}

void VertexBuffer::bind(const VertexBuffer *vertexBuffer) {
    if (vertexBuffer != nullptr)
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_VBO);
    else
        glBindBuffer(GL_ARRAY_BUFFER, 0);
}
