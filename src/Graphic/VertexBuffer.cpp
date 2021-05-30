#include <Graphic/VertexBuffer.hpp>
#include <iostream>

VertexBuffer::VertexBuffer() : m_VBO(0) {}

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &m_VBO); }

VertexBuffer::VertexBuffer(const VertexBuffer &other)
    : BufferObject(other), m_VBO(0) {
    initialize();
    glBindBuffer(GL_COPY_READ_BUFFER, other.m_VBO);

    int size;
    glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
    glBindBuffer(GL_COPY_WRITE_BUFFER, m_VBO);
    glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, other.m_mode);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

    bindVertexArray();
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    Vertex::setupAttribute();
}

VertexBuffer::VertexBuffer(VertexBuffer &&other) { swap(*this, other); }

VertexBuffer &VertexBuffer::operator=(VertexBuffer other) {
    swap(*this, other);
    return *this;
}

void swap(VertexBuffer &first, VertexBuffer &second) {
    using std::swap;
    swap(first.m_VBO, second.m_VBO);
    swap(static_cast<BufferObject &>(first),
         static_cast<BufferObject &>(second));
}

bool VertexBuffer::initialize() {
    if (isInit()) return true;
    glGenBuffers(1, &m_VBO);
    if (!m_VBO) {
        std::cerr << "Could not create vertex buffer." << std::endl;
        return false;
    }
    return BufferObject::initialize();
}

void VertexBuffer::drawPrimitive() const {
    bindVertexArray();
    glDrawArrays(m_primitiveType, 0, m_size);
}

void VertexBuffer::update(const Vertex *vertices, std::size_t cnt, GLenum type,
                          GLenum mode) {
    bindVertexArray();
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * cnt, vertices, mode);
    m_size = cnt;
    m_primitiveType = type;
    Vertex::setupAttribute();
}
