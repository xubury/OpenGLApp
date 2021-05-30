#include <Graphic/BufferObject.hpp>
#include <Graphic/RenderTarget.hpp>
#include <iostream>

BufferObject::BufferObject()
    : m_size(0),
      m_mode(GL_STATIC_DRAW),
      m_primitiveType(GL_TRIANGLES),
      m_VAO(0) {}

BufferObject::~BufferObject() { glDeleteVertexArrays(1, &m_VAO); }

BufferObject::BufferObject(const BufferObject &other)
    : m_size(other.m_size),
      m_mode(other.m_mode),
      m_primitiveType(other.m_primitiveType),
      m_VAO(0) {}

BufferObject::BufferObject(BufferObject &&other) { swap(*this, other); }

void BufferObject::draw(RenderTarget &target, RenderStates states) const {
    target.draw(*this, states);
}

void swap(BufferObject &first, BufferObject &second) {
    using std::swap;
    swap(first.m_VAO, second.m_VAO);
    swap(first.m_size, second.m_size);
    swap(first.m_mode, second.m_mode);
    swap(first.m_primitiveType, second.m_primitiveType);
}

bool BufferObject::isInit() const { return m_VAO != 0; }

bool BufferObject::initialize() {
    glGenVertexArrays(1, &m_VAO);
    if (!m_VAO) {
        std::cerr << "Could not create vertex array object." << std::endl;
        return false;
    }
    return true;
}

std::size_t BufferObject::size() const { return m_size; }

bool BufferObject::empty() const { return m_size == 0; }

void BufferObject::bindVertexArray() const { glBindVertexArray(m_VAO); }
