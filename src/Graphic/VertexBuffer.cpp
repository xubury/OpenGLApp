#include <Graphic/VertexBuffer.hpp>
#include <Graphic/RenderStates.hpp>
#include <Graphic/RenderTarget.hpp>

VertexBuffer::VertexBuffer(int type)
    : m_VBO(0), m_VAO(0), m_size(0), m_drawType(type) {}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

std::size_t VertexBuffer::size() const { return m_size; }

bool VertexBuffer::empty() const { return m_size == 0; }

bool VertexBuffer::isInit() const { return m_VBO != 0; }

void VertexBuffer::draw(RenderTarget &target, RenderStates states) const {
    target.draw(*this, states);
}

void VertexBuffer::drawPrimitive() const {
    glBindVertexArray(m_VAO);
    glDrawArrays(m_drawType, 0, m_size);
}
