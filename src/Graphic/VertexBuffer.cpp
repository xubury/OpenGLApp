#include <Graphic/VertexBuffer.hpp>
#include <Graphic/RenderStates.hpp>
#include <Graphic/RenderTarget.hpp>

VertexBuffer::VertexBuffer() : m_VBO(0), m_size(0) {}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_VBO); }

std::size_t VertexBuffer::size() const { return m_size; }

bool VertexBuffer::empty() const { return m_size == 0; }

void VertexBuffer::draw(RenderTarget &target, RenderStates states) const {
    target.draw(*this, states);
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_VBO); }
