#include "Graphic/ElementBuffer.hpp"
#include <iostream>

ElementBuffer::ElementBuffer() : m_VBO(0), m_EBO(0) {
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
}

ElementBuffer::~ElementBuffer() {
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

ElementBuffer::ElementBuffer(const ElementBuffer &other)
    : BufferObject(other), m_VBO(0), m_EBO(0) {
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    int size;
    glBindBuffer(GL_COPY_READ_BUFFER, other.m_VBO);
    glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
    glBindBuffer(GL_COPY_WRITE_BUFFER, m_VBO);
    glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, other.m_mode);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

    glBindBuffer(GL_COPY_READ_BUFFER, other.m_EBO);
    glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
    glBindBuffer(GL_COPY_WRITE_BUFFER, m_EBO);
    glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, other.m_mode);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

    bindVertexArray();
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    Vertex::setupAttribute();
}

ElementBuffer::ElementBuffer(ElementBuffer &&other) { swap(*this, other); }

ElementBuffer &ElementBuffer::operator=(ElementBuffer other) {
    swap(*this, other);
    return *this;
}

void swap(ElementBuffer &first, ElementBuffer &second) {
    using std::swap;
    swap(first.m_VBO, second.m_VBO);
    swap(first.m_EBO, second.m_EBO);
    swap(static_cast<BufferObject &>(first),
         static_cast<BufferObject &>(second));
}

void ElementBuffer::drawPrimitive() const {
    bindVertexArray();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glDrawElements(m_primitiveType, m_size, GL_UNSIGNED_INT, 0);
}

void ElementBuffer::update(const Vertex *vertices, std::size_t vertexCnt,
                           const uint32_t *indices, std::size_t indexCnt,
                           GLenum type, GLenum mode) {
    update(vertices, vertexCnt, type, mode);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indexCnt, indices,
                 mode);
    m_size = indexCnt;
}

void ElementBuffer::update(const Vertex *vertices, std::size_t vertexCnt,
                           GLenum type, GLenum mode) {
    bindVertexArray();
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCnt * sizeof(Vertex), vertices, mode);
    m_primitiveType = type;
    m_mode = mode;
    Vertex::setupAttribute();
}
