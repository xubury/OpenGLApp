#include "Graphic/Buffer.hpp"

namespace te {

VertexBuffer::VertexBuffer(const void *vertices, std::size_t size)
    : m_size(size) {
    glCreateBuffers(1, &m_bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_bufferId); }

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_bufferId); }

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

const BufferLayout &VertexBuffer::getLayout() const { return m_layout; }

void VertexBuffer::setLayout(const BufferLayout &layout) { m_layout = layout; }

std::size_t VertexBuffer::getSize() const { return m_size; }

void VertexBuffer::update(const void *vertices, std::size_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
    m_size = size;
}

IndexBuffer::IndexBuffer(const uint32_t *indices, std::size_t count)
    : m_count(count) {
    glCreateBuffers(1, &m_bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
                 GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &m_bufferId); }

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
}

void IndexBuffer::unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

std::size_t IndexBuffer::getCount() const { return m_count; }

void IndexBuffer::update(const uint32_t *indices, std::size_t count) {
    glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
                 GL_STATIC_DRAW);
    m_count = count;
}

}  // namespace te
