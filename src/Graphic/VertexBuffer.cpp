#include <Graphic/VertexBuffer.hpp>
#include <Graphic/RenderStates.hpp>
#include <Graphic/RenderTarget.hpp>

VertexBuffer::VertexBuffer()
    : m_VBO(0),
      m_VAO(0),
      m_size(0),
      m_mode(GL_STATIC_DRAW),
      m_primitiveType(GL_TRIANGLES),
      m_attrFunction(nullptr) {}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

VertexBuffer::VertexBuffer(const VertexBuffer &other)
    : m_VBO(0),
      m_VAO(0),
      m_size(other.m_size),
      m_mode(other.m_mode),
      m_primitiveType(other.m_primitiveType),
      m_attrFunction(other.m_attrFunction) {
    initialize();
    glBindBuffer(GL_COPY_READ_BUFFER, other.m_VBO);

    int size;
    glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);
    glBindBuffer(GL_COPY_WRITE_BUFFER, m_VBO);
    glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, other.m_mode);
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    m_attrFunction();
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer other) {
    std::swap(m_VAO, other.m_VAO);
    std::swap(m_VBO, other.m_VBO);
    std::swap(m_size, other.m_size);
    std::swap(m_mode, other.m_mode);
    std::swap(m_primitiveType, other.m_primitiveType);
    std::swap(m_attrFunction, other.m_attrFunction);
    return *this;
}

bool VertexBuffer::initialize() {
    if (isInit()) return true;
    glGenBuffers(1, &m_VBO);
    if (!m_VBO) {
        std::cerr << "Could not create vertex buffer." << std::endl;
        return false;
    }
    glGenVertexArrays(1, &m_VAO);
    if (!m_VAO) {
        std::cerr << "Could not create vertex array object." << std::endl;
        return false;
    }
    return true;
}

std::size_t VertexBuffer::size() const { return m_size; }

bool VertexBuffer::empty() const { return m_size == 0; }

bool VertexBuffer::isInit() const { return m_VAO != 0; }

void VertexBuffer::draw(RenderTarget &target, RenderStates states) const {
    target.draw(*this, states);
}

void VertexBuffer::drawPrimitive() const {
    glBindVertexArray(m_VAO);
    glDrawArrays(m_primitiveType, 0, m_size);
}
