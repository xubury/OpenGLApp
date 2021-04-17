#include <glad/glad.h>
#include <iostream>

#include <Graphic/ElementBuffer.hpp>
#include <Graphic/RenderTarget.hpp>

ElementBuffer::ElementBuffer() : m_VBO(0), m_EBO(0) {}

ElementBuffer::~ElementBuffer() {
    glDeleteBuffers(1, &m_EBO);
    glDeleteBuffers(1, &m_VBO);
}

std::size_t ElementBuffer::size() const { return m_size; }

bool ElementBuffer::empty() const { return m_size == 0; }

bool ElementBuffer::create(const Vertex *vertices, std::size_t vertexSize,
                           uint32_t *indices, std::size_t indexSize) {
    glGenBuffers(1, &m_EBO);
    glGenBuffers(1, &m_VBO);
    if (!m_VBO || !m_EBO) {
        std::cerr << "Could not create vertex buffer array." << std::endl;
        return false;
    }
    update(vertices, vertexSize, indices, indexSize);
    return true;
}

void ElementBuffer::update(const Vertex *vertices, std::size_t vertexSize,
                           const uint32_t *indices, std::size_t indexSize) {
    update(vertices, vertexSize);
    update(indices, indexSize);
    m_size = indexSize;
}

void ElementBuffer::update(const Vertex *vertices, std::size_t vertexSize) {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexSize, vertices,
                 GL_STATIC_DRAW);
}

void ElementBuffer::update(const uint32_t *indices, std::size_t indexSize) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indexSize, indices,
                 GL_STATIC_DRAW);
}

void ElementBuffer::draw(RenderTarget &target, RenderStates states) const {
    target.draw(*this, states);
}

void ElementBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
}
