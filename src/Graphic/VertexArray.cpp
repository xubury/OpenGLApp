#include "Graphic/VertexArray.hpp"
#include "Graphic/OpenGL.hpp"

namespace te {

static inline GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:
            return GL_FLOAT;
        case ShaderDataType::Float2:
            return GL_FLOAT;
        case ShaderDataType::Float3:
            return GL_FLOAT;
        case ShaderDataType::Float4:
            return GL_FLOAT;
        case ShaderDataType::Mat3:
            return GL_FLOAT;
        case ShaderDataType::Mat4:
            return GL_FLOAT;
        case ShaderDataType::Int:
            return GL_INT;
        case ShaderDataType::Int2:
            return GL_INT;
        case ShaderDataType::Int3:
            return GL_INT;
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
        default:
            TE_CORE_ASSERT(false, "Unknown ShaderDataType!");
    }

    return {};
}

VertexArray::VertexArray() : m_arrayId(0), m_vertexBufferId(0) {
    glCreateVertexArrays(1, &m_arrayId);
}

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &m_arrayId); }

void VertexArray::bind() const { glBindVertexArray(m_arrayId); }

void VertexArray::unbind() const { glBindVertexArray(0); }

void VertexArray::addVertexBuffer(const Ref<VertexBuffer> &buffer) {
    bind();
    buffer->bind();
    const auto &layout = buffer->getLayout();
    for (const auto &element : layout) {
        switch (element.getType()) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4: {
                glEnableVertexAttribArray(m_vertexBufferId);
                glVertexAttribPointer(
                    m_vertexBufferId, element.getComponentCount(),
                    shaderDataTypeToOpenGLBaseType(element.getType()),
                    element.isNormalized() ? GL_TRUE : GL_FALSE,
                    layout.getStride(), (const void *)element.getOffset());
                m_vertexBufferId++;
                break;
            }
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::Bool: {
                glEnableVertexAttribArray(m_vertexBufferId);
                glVertexAttribIPointer(
                    m_vertexBufferId, element.getComponentCount(),
                    shaderDataTypeToOpenGLBaseType(element.getType()),
                    layout.getStride(), (const void *)element.getOffset());
                m_vertexBufferId++;
                break;
            }
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4: {
                uint8_t count = element.getComponentCount();
                for (uint8_t i = 0; i < count; i++) {
                    glEnableVertexAttribArray(m_vertexBufferId);
                    glVertexAttribPointer(
                        m_vertexBufferId, count,
                        shaderDataTypeToOpenGLBaseType(element.getType()),
                        element.isNormalized() ? GL_TRUE : GL_FALSE,
                        layout.getStride(),
                        (const void *)(element.getOffset() +
                                       sizeof(float) * count * i));
                    glVertexAttribDivisor(m_vertexBufferId, 1);
                    m_vertexBufferId++;
                }
                break;
            }
            default:
                TE_CORE_ASSERT(false, "Unknown ShaderDataType!");
        }
    }
    m_vertexBuffers.push_back(buffer);
}

void VertexArray::setIndexBuffer(const Ref<IndexBuffer> &buffer) {
    bind();
    buffer->bind();
    m_indexBuffer = buffer;
}

const std::vector<Ref<VertexBuffer>> &VertexArray::getVertexBuffers() const {
    return m_vertexBuffers;
}

const Ref<IndexBuffer> &VertexArray::getIndexBuffer() const {
    return m_indexBuffer;
}

}  // namespace te
