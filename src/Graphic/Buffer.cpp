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

uint32_t UniformBuffer::s_count = 0;

UniformBuffer::UniformBuffer(std::size_t size) : m_size(size) {
    glCreateBuffers(1, &m_bufferId);
    glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
    m_slot = s_count;
    glBindBufferRange(GL_UNIFORM_BUFFER, m_slot, m_bufferId, 0, size);
    ++s_count;
}

UniformBuffer::~UniformBuffer() { glDeleteBuffers(1, &m_bufferId); }

void UniformBuffer::setData(const void *data, std::size_t offset,
                            std::size_t size) {
    bind();
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void UniformBuffer::bind() const {
    glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
}

void UniformBuffer::unbind() const { glBindBuffer(GL_UNIFORM_BUFFER, 0); }

uint32_t UniformBuffer::getBindingPoint() const { return m_slot; }

}  // namespace te
