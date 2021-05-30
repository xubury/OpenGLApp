#include <Graphic/ElementBuffer.hpp>
#include <iostream>

ElementBuffer::ElementBuffer() : m_VBO(0), m_EBO(0) {}

ElementBuffer::~ElementBuffer() {
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

ElementBuffer::ElementBuffer(const ElementBuffer &other)
    : BufferObject(other), m_VBO(0), m_EBO(0) {
    initialize();

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
    m_attrFunction();
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

bool ElementBuffer::initialize() {
    if (isInit()) return true;
    glGenBuffers(1, &m_VBO);
    if (!m_VBO) {
        std::cerr << "Could not create vertex buffer." << std::endl;
        return false;
    }
    glGenBuffers(1, &m_EBO);
    if (!m_EBO) {
        std::cerr << "Could not create element buffer object." << std::endl;
        return false;
    }
    return BufferObject::initialize();
}

void ElementBuffer::drawPrimitive() const {
    bindVertexArray();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glDrawElements(m_primitiveType, m_size, GL_UNSIGNED_INT, 0);
}
