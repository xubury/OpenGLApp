#include "Graphic/Layout.hpp"
namespace te {

static inline uint32_t shaderDataTypeSize(ShaderDataType type) {
    switch (type) {
        case ShaderDataType::Float:
            return 4;
        case ShaderDataType::Float2:
            return 4 * 2;
        case ShaderDataType::Float3:
            return 4 * 3;
        case ShaderDataType::Float4:
            return 4 * 4;
        case ShaderDataType::Mat3:
            return 4 * 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4 * 4;
        case ShaderDataType::Int:
            return 4;
        case ShaderDataType::Int2:
            return 4 * 2;
        case ShaderDataType::Int3:
            return 4 * 3;
        case ShaderDataType::Int4:
            return 4 * 4;
        case ShaderDataType::Bool:
            return 1;
        default:
            TE_CORE_ASSERT(false, "Unknown ShaderDataType!");
    }
    return 0;
}

BufferElement::BufferElement(ShaderDataType type, const std::string &name,
                             bool normalized)
    : m_type(type),
      m_name(name),
      m_size(shaderDataTypeSize(type)),
      m_offset(0),
      m_normalized(normalized) {}

uint32_t BufferElement::getComponentCount() const {
    switch (m_type) {
        case ShaderDataType::Float:
            return 1;
        case ShaderDataType::Float2:
            return 2;
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Float4:
            return 4;
        case ShaderDataType::Mat3:
            return 3;  // 3* float3
        case ShaderDataType::Mat4:
            return 4;  // 4* float4
        case ShaderDataType::Int:
            return 1;
        case ShaderDataType::Int2:
            return 2;
        case ShaderDataType::Int3:
            return 3;
        case ShaderDataType::Int4:
            return 4;
        case ShaderDataType::Bool:
            return 1;
        default:
            TE_CORE_ASSERT(false, "Unknown ShaderDataType!");
    }
    return 0;
}

ShaderDataType BufferElement::getType() const { return m_type; }

uint32_t BufferElement::getSize() const { return m_size; }

std::size_t BufferElement::getOffset() const { return m_offset; }

bool BufferElement::isNormalized() const { return m_normalized; }

BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
    : m_elements(elements), m_stride(0) {
    calculateOffsetAndStride();
}

void BufferLayout::calculateOffsetAndStride() {
    std::size_t offset = 0;
    for (auto &element : m_elements) {
        element.m_offset += offset;
        offset += element.m_size;
        m_stride += element.m_size;
    }
}
std::vector<BufferElement>::iterator BufferLayout::begin() {
    return m_elements.begin();
}
std::vector<BufferElement>::iterator BufferLayout::end() {
    return m_elements.end();
}

std::vector<BufferElement>::const_iterator BufferLayout::begin() const {
    return m_elements.begin();
}

std::vector<BufferElement>::const_iterator BufferLayout::end() const {
    return m_elements.end();
}

std::size_t BufferLayout::getStride() const { return m_stride; }

}  // namespace te
